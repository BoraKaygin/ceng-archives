{-# LANGUAGE FlexibleInstances #-}

module PE3 where

import Data.List (sort, sortBy)
import Text.Printf (printf)
import GHC.Integer (neqInteger)

data Term = Const Integer | Pw Integer Power | Trig Integer Power Trigonometric | Exp Integer Power Exponential --deriving Show

data Power = Power Integer
data Polynomial = Polynomial [(Integer, Power)]
data Exponential = Exponential Polynomial
data Trigonometric = Sin Polynomial | Cos Polynomial

class Evaluable a where
    function :: a -> (Integer -> Double)

class Differentiable a where
    derivative :: a -> [Term]

-- You can use this as is
getRounded :: Double -> Double
getRounded x = read s :: Double
               where s = printf "%.2f" x

-- You don't have to follow the order the functions appear in the file
-- For example, you could first define all Show instances, then all Eq instances etc.
-- if that implementation order is more convenient for you.



-- INSTANCES FOR POWER

instance Show Power where
    show (Power n) | n == 0 = "1"
                   | n == 1 = "x"
                   | otherwise = "x^" ++ show n

instance Eq Power where
    Power a == Power b = a == b

instance Ord Power where
    Power a < Power b = a < b
    Power a <= Power b = a <= b
    Power a > Power b = a > b
    Power a >= Power b = a >= b

instance Evaluable Power where
    function (Power n) x = getRounded $ powEval (Power n) x

powEval :: Power -> Integer -> Double
powEval (Power n) x = fromInteger x ** fromInteger n

instance Differentiable Power where
    derivative power = termListSimplifier $ derivePower power

derivePower :: Power -> [Term]
derivePower (Power n) | n == 0 = [Const 0]
                      | n == 1 = [Const 1]
                      | otherwise = [Pw n (Power (n-1))]



-- INSTANCES FOR POLYNOMIAL

instance Show Polynomial where
    show polynomial = let printed = showPoly polynomial
                       in if printed == "" then "0" else printed

showPoly :: Polynomial -> String
showPoly (Polynomial []) = ""
showPoly (Polynomial [(coefficient, power)]) | coefficient == 0 = ""
                                             | power == Power 0 = show coefficient
                                             | coefficient == 1 = show power
                                             | coefficient == -1 = "-" ++ show power
                                             | otherwise = show coefficient ++ show power
showPoly (Polynomial ((coefficient, power):rest)) | coefficient == 0 = showPoly (Polynomial rest)
                                                  | power == Power 0 = if showPoly (Polynomial rest) /= "" then show coefficient ++ " + " ++ showPoly (Polynomial rest) else show coefficient
                                                  | coefficient == 1 = if showPoly (Polynomial rest) /= "" then show power ++ " + " ++ showPoly (Polynomial rest) else show power
                                                  | coefficient == -1 = if showPoly (Polynomial rest) /= "" then "-" ++ show power ++ " + " ++ showPoly (Polynomial rest) else "-" ++ show power
                                                  | otherwise = if showPoly (Polynomial rest) /= "" then show coefficient ++ show power ++ " + " ++ showPoly (Polynomial rest) else show coefficient ++ show power

instance Eq Polynomial where
    Polynomial a == Polynomial b = sort a == sort b

instance Evaluable Polynomial where
    function (Polynomial []) _ = 0.0
    function polynomial x = getRounded $ polyEval polynomial x

polyEval :: Polynomial -> Integer -> Double
polyEval (Polynomial ((coefficient, power):rest)) x = (fromInteger coefficient * function power x) + function (Polynomial rest) x

instance Differentiable Polynomial where
    derivative polynomial = termListSimplifier $ derivePolynomial polynomial

derivePolynomial :: Polynomial -> [Term]
derivePolynomial (Polynomial []) = []
derivePolynomial (Polynomial ((coefficient, power):rest)) | power == Power 0 = derivePolynomial (Polynomial rest)
                                                          | power == Power 1 = Const coefficient : derivePolynomial (Polynomial rest)
                                                          | otherwise = Pw (coefficient*pow) (Power (pow-1)) : derivePolynomial (Polynomial rest)
                                                            where Power pow = power




-- INSTANCES FOR TRIGONOMETRIC

instance Show Trigonometric where
    show (Sin polynomial@(Polynomial [(coefficient,Power 0)])) = if coefficient>0 then "sin" ++ show polynomial else "sin(" ++ show polynomial ++ ")"
    show (Sin polynomial@(Polynomial [(coefficient,Power 1)])) = if coefficient>0 then "sin" ++ show polynomial else "sin(" ++ show polynomial ++ ")"
    show (Sin polynomial) = "sin(" ++ show polynomial ++ ")"
    show (Cos polynomial@(Polynomial [(coefficient,Power 0)])) = if coefficient>0 then "cos" ++ show polynomial else "cos(" ++ show polynomial ++ ")"
    show (Cos polynomial@(Polynomial [(coefficient,Power 1)])) = if coefficient>0 then "cos" ++ show polynomial else "cos(" ++ show polynomial ++ ")"
    show (Cos polynomial) = "cos(" ++ show polynomial ++ ")"

instance Eq Trigonometric where
    Sin a == Sin b = a == b
    Cos a == Cos b = a == b
    _ == _ = False

instance Evaluable Trigonometric where
    function (Sin polynomial) x = getRounded $ sinEval (Sin polynomial) x
    function (Cos polynomial) x = getRounded $ cosEval (Cos polynomial) x

sinEval :: Trigonometric -> Integer -> Double
sinEval (Sin polynomial) x = sin (polyEval polynomial x)
cosEval :: Trigonometric -> Integer -> Double
cosEval (Cos polynomial) x = cos (polyEval polynomial x)

instance Differentiable Trigonometric where
    derivative trigonometric = termListSimplifier $ deriveTrigonometric trigonometric

deriveTrigonometric :: Trigonometric -> [Term]
deriveTrigonometric (Sin polynomial) = let diffInside = derivative polynomial
                                           diffOutside = Cos polynomial
                                        in if null diffInside then [Const 0] else trigPutTogether diffInside diffOutside
deriveTrigonometric (Cos polynomial) = let diffInside = derivative polynomial
                                           diffOutside = Sin polynomial
                                        in if null diffInside then [Const 0] else trigPutTogether (takeNegative diffInside) diffOutside

takeNegative :: [Term] -> [Term]
takeNegative [] = []
takeNegative (firstTerm:rest) = case firstTerm of Const 0 -> Const 0 : takeNegative rest
                                                  Const num -> Const (-num) : takeNegative rest
                                                  Pw coefficient power -> Pw (-coefficient) power : takeNegative rest

trigPutTogether :: [Term] -> Trigonometric -> [Term]
trigPutTogether [] _ = []
trigPutTogether (firstTerm:rest) trigonometric = case firstTerm of Const num -> if num == 0 then Const 0 : trigPutTogether rest trigonometric else Trig num (Power 0) trigonometric : trigPutTogether rest trigonometric
                                                                   Pw coefficient power -> Trig coefficient power trigonometric : trigPutTogether rest trigonometric

-- INSTANCES FOR EXPONENTIAL

instance Show Exponential where
    show (Exponential polynomial@(Polynomial [(coefficient,Power 0)])) = if coefficient>0 then "e^" ++ show polynomial else "e^(" ++ show polynomial ++ ")"
    show (Exponential polynomial@(Polynomial [(coefficient,Power 1)])) = if coefficient>0 then "e^" ++ show polynomial else "e^(" ++ show polynomial ++ ")"
    show (Exponential polynomial) = "e^(" ++ show polynomial ++ ")"

instance Eq Exponential where
    Exponential a == Exponential b = a == b


instance Evaluable Exponential where
    function (Exponential polynomial) x = getRounded $ expEval (Exponential polynomial) x

expEval :: Exponential -> Integer -> Double
expEval (Exponential polynomial) x = exp (polyEval polynomial x)

instance Differentiable Exponential where
    derivative exponential = termListSimplifier $ deriveExponential exponential

deriveExponential :: Exponential -> [Term]
deriveExponential (Exponential polynomial) = let diffInside = derivative polynomial
                                                 diffOutside = Exponential polynomial
                                              in if null diffInside then [Const 0] else expPutTogether diffInside diffOutside

expPutTogether :: [Term] -> Exponential -> [Term]
expPutTogether [] _ = []
expPutTogether (firstTerm:rest) exponential = case firstTerm of Const num -> if num == 0 then Const 0 : expPutTogether rest exponential else Exp num (Power 0) exponential : expPutTogether rest exponential
                                                                Pw coefficient power -> Exp coefficient power exponential : expPutTogether rest exponential

-- INSTANCES FOR TERM

instance Show Term where
    show (Const a) = show a
    show (Pw coefficient (Power pow)) | coefficient == 0 = "0"
                                      | pow == 0 = show coefficient
                                      | coefficient == 1 = show (Power pow)
                                      | coefficient == (-1) = "-" ++ show (Power pow)
                                      | otherwise = show coefficient ++ show (Power pow)
    show (Exp coefficient (Power pow) exponential) | coefficient == 0 = "0"
                                                   | coefficient == 1 && pow == 0 = show exponential
                                                   | coefficient == (-1) && pow == 0 = "-" ++ show exponential
                                                   | pow == 0 = show coefficient ++ show exponential
                                                   | coefficient == 1 = show (Power pow) ++ show exponential
                                                   | coefficient == (-1) = "-" ++ show (Power pow) ++ show exponential
                                                   | otherwise = show coefficient ++ show (Power pow) ++ show exponential
    show (Trig coefficient (Power pow) trigonometric) | coefficient == 0 = "0"
                                                      | coefficient == 1 && pow == 0 = show trigonometric
                                                      | coefficient == (-1) && pow == 0 = "-" ++ show trigonometric
                                                      | pow == 0 = show coefficient ++ show trigonometric
                                                      | coefficient == 1 = show (Power pow) ++ show trigonometric
                                                      | coefficient == (-1) = "-" ++ show (Power pow) ++ show trigonometric
                                                      | otherwise = show coefficient ++ show (Power pow) ++ show trigonometric

instance Eq Term where
    Const a == Const b = a == b
    Pw a b == Pw c d = a == c && b == d
    Exp a b c == Exp d e f = a == d && b == e && c == f
    Trig a b c == Trig d e f = a == d && b == e && c == f
    _ == _ = False

instance Evaluable Term where
    function (Const integer) _ = fromInteger integer
    function (Pw coefficient power) x = getRounded $ pwTermEval (Pw coefficient power) x
    function (Exp coefficient power exponential) x = getRounded $ expTermEval (Exp coefficient power exponential) x
    function (Trig coefficient power trigonometric) x = getRounded $ trigTermEval (Trig coefficient power trigonometric) x

pwTermEval :: Term -> Integer -> Double
pwTermEval (Pw coefficient power) x = fromInteger coefficient * function power x
expTermEval :: Term -> Integer -> Double
expTermEval (Exp coefficient power exponential) x = fromInteger coefficient * powEval power x * expEval exponential x
trigTermEval :: Term -> Integer -> Double
trigTermEval (Trig coefficient power trigonometric) x = fromInteger coefficient * powEval power x * case trigonometric of Sin polynomial -> sinEval (Sin polynomial) x
                                                                                                                          Cos polynomial -> cosEval (Cos polynomial) x

instance Differentiable Term where
    derivative (Const _) = [Const 0]
    derivative (Pw coefficient power) = termListSimplifier $ derivePw (Pw coefficient power)
    derivative (Exp coefficient power exponential) = termListSimplifier $ deriveExp (Exp coefficient power exponential)
    derivative (Trig coefficient power exponential) = termListSimplifier $ deriveTrig (Trig coefficient power exponential)

derivePw :: Term -> [Term]
derivePw (Pw coefficient power) | diffPow == [Const 0] = [Const 0]
                                | diffPow == [Const 1] = [Const coefficient]
                                | otherwise = let Power pow = power
                                               in [Pw (coefficient * pow) (Power (pow - 1))]
                                  where diffPow = derivative power
deriveExp :: Term -> [Term]
deriveExp (Exp coefficient power exponential) = let f = Pw coefficient power
                                                    f' = derivative f
                                                    productRuleFirst = expPutTogether f' exponential
                                                    g = case exponential of Exponential polynomial -> polynomial
                                                    g' = derivative g
                                                    fg' = map (multiplyTerm f) g'
                                                    productRuleSecond = expPutTogether fg' exponential
                                                 in productRuleFirst ++ productRuleSecond

deriveTrig :: Term -> [Term]
deriveTrig (Trig coefficient power trigonometric) = let f = Pw coefficient power
                                                        f' = derivative f
                                                        first = trigPutTogether f' trigonometric
                                                        g = case trigonometric of Sin polynomial -> polynomial
                                                                                  Cos polynomial -> polynomial
                                                        g' = derivative g
                                                        fg' = map (multiplyTerm f) g'
                                                        second = case trigonometric of Sin polynomial -> trigPutTogether fg' (Cos polynomial)
                                                                                       Cos polynomial -> trigPutTogether (takeNegative fg') (Sin polynomial)
                                                        in first ++ second


multiplyTerm :: Term -> Term -> Term
multiplyTerm (Const coefficient1) (Const coefficient2) = Const (coefficient1 * coefficient2)
multiplyTerm (Const coefficient1) (Pw coefficient2 power2) = Pw (coefficient1 * coefficient2) power2
multiplyTerm (Pw coefficient1 power1) (Const coefficient2) = Pw (coefficient1 * coefficient2) power1
multiplyTerm (Pw coefficient1 power1) (Pw coefficient2 power2) = let Power pow1 = power1
                                                                     Power pow2 = power2
                                                                  in Pw (coefficient1 * coefficient2) (Power (pow1 + pow2))

turnIntoExp :: Term -> Exponential -> Term
turnIntoExp (Pw coefficient power) = Exp coefficient power

termListSimplifier :: [Term] -> [Term]
termListSimplifier terms = emptyToZero $ removeZeroes $ combineDuplicates $ removeZeroes terms

emptyToZero :: [Term] -> [Term]
emptyToZero terms = if null terms then [Const 0] else terms

removeZeroes :: [Term] -> [Term]
removeZeroes [] = []
removeZeroes (Const 0:rest) = removeZeroes rest
removeZeroes (Pw 0 _:rest) = removeZeroes rest
removeZeroes (Exp 0 _ _:rest) = removeZeroes rest
removeZeroes (Trig 0 _ _:rest) = removeZeroes rest
removeZeroes (first:rest) = first : removeZeroes rest

combineDuplicates :: [Term] -> [Term]
combineDuplicates [] = []
combineDuplicates (first:rest) = let duplicate = findDuplicate first rest
                                     newFirst = if duplicate == Const 0 then first else addTerms first duplicate
                                     newRest = if duplicate == Const 0 then rest else deleteDuplicate duplicate rest
                                  in if isZero newFirst then combineDuplicates newRest else newFirst : combineDuplicates newRest

addTerms :: Term -> Term -> Term
addTerms (Const num) (Const num2) = Const (num + num2)
addTerms (Pw coefficient power) (Pw coefficient2 power2) = Pw (coefficient + coefficient2) power
addTerms (Exp coefficient power exponential) (Exp coefficient2 power2 exponential2) = Exp (coefficient + coefficient2) power exponential
addTerms (Trig coefficient power trigonometric) (Trig coefficient2 power2 trigonometric2) = Trig (coefficient + coefficient2) power trigonometric

isZero :: Term -> Bool
isZero (Const num) = num == 0
isZero (Pw coefficient power) = coefficient == 0
isZero (Exp coefficient power exponential) = coefficient == 0
isZero (Trig coefficient power trigonometric) = coefficient == 0

findDuplicate :: Term -> [Term] -> Term
findDuplicate _ [] = Const 0
findDuplicate (Const num) (firstTerm:rest) = case firstTerm of Const num2 -> Const num2
                                                               _ -> findDuplicate (Const num) rest
findDuplicate (Pw coefficient power) (firstTerm:rest) = case firstTerm of Pw coefficient2 power2 -> if power == power2 then Pw coefficient2 power2 else findDuplicate (Pw coefficient power) rest
                                                                          _ -> findDuplicate (Pw coefficient power) rest
findDuplicate (Exp coefficient power exponential) (firstTerm:rest) = case firstTerm of Exp coefficient2 power2 exponential2 -> if power==power2 && exponential==exponential2 then Exp coefficient2 power2 exponential2 else findDuplicate (Exp coefficient power exponential) rest
                                                                                       _ -> findDuplicate (Exp coefficient power exponential) rest
findDuplicate (Trig coefficient power trigonometric) (firstTerm:rest) = case firstTerm of Trig coefficient2 power2 trigonometric2 -> if power==power2 && trigonometric==trigonometric2 then Trig coefficient2 power2 trigonometric2 else findDuplicate (Trig coefficient power trigonometric) rest
                                                                                          _ -> findDuplicate (Trig coefficient power trigonometric) rest

deleteDuplicate :: Term -> [Term] -> [Term]
deleteDuplicate _ [] = []
deleteDuplicate (Const num) (firstTerm:rest) = case firstTerm of Const num -> rest
                                                                 _ -> firstTerm : deleteDuplicate (Const num) rest
deleteDuplicate (Pw coefficient power) (firstTerm:rest) = case firstTerm of Pw coefficient power2 -> if power==power2 then rest else firstTerm : deleteDuplicate (Pw coefficient power) rest
                                                                            _ -> firstTerm : deleteDuplicate (Pw coefficient power) rest
deleteDuplicate (Exp coefficient power exponential) (firstTerm:rest) = case firstTerm of Exp coefficient power2 exponential2 -> if power==power2 && exponential==exponential2 then rest else firstTerm : deleteDuplicate (Exp coefficient power exponential) rest
                                                                                         _ -> firstTerm : deleteDuplicate (Exp coefficient power exponential) rest
deleteDuplicate (Trig coefficient power trigonometric) (firstTerm:rest) = case firstTerm of Trig coefficient power2 trigonometric2 -> if power==power2 && trigonometric==trigonometric2 then rest else firstTerm : deleteDuplicate (Trig coefficient power trigonometric) rest
                                                                                            _ -> firstTerm : deleteDuplicate (Trig coefficient power trigonometric) rest


-- INSTANCES FOR [TERM]

instance Evaluable [Term] where
    function terms x = getRounded $ evalTerms terms x

evalTerms :: [Term] -> Integer -> Double
evalTerms [] _ = 0.0
evalTerms (Const num:rest) x = fromInteger num + evalTerms rest x
evalTerms (pw@(Pw coefficient power):rest) x = pwTermEval pw x + evalTerms rest x
evalTerms (exp@(Exp coefficient power exponential):rest) x = expTermEval exp x + evalTerms rest x
evalTerms (trig@(Trig coefficient power trigonometric):rest) x = trigTermEval trig x + evalTerms rest x

instance Differentiable [Term] where
    derivative terms = termListSimplifier $ deriveTerms $ termListSimplifier terms

deriveTerms :: [Term] -> [Term]
deriveTerms [] = []
deriveTerms (Const num:rest) = Const 0 : deriveTerms rest
deriveTerms (pw@(Pw coefficient power):rest) = derivePw pw ++ deriveTerms rest
deriveTerms (exp@(Exp coefficient power exponential):rest) = deriveExp exp ++ deriveTerms rest
deriveTerms (trig@(Trig coefficient power trigonometric):rest) = deriveTrig trig ++ deriveTerms rest
