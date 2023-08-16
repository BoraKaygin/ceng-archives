module PE1 where

import Text.Printf

-- PE1: Recipe Calculator
-- The premise of this homework if to write a recipe calculator that
-- calculates: how much a recipe costs to make, what can be made with the
-- ingredients already available, and how much extra ingredients need to
-- be bought in order to make a recipe.

-- Recipe = Recipe Name [(Ingredient, Quantity)]
data Recipe = Recipe String [(String, Double)] deriving Show

-- Price = Price Ingredient Quantity Price
data Price = Price String Double Double deriving Show

-- You can use this as-is
getRounded :: Double -> Double
getRounded x = read s :: Double
               where s = printf "%.2f" x

-- Calculate how much the given amount of the given ingredient costs
getIngredientCost :: (String, Double) -> [Price] -> Double
getIngredientCost (name, quantity) ((Price lname lquantity lprice) : tail) = getRounded (getIngredientCost' (name, quantity) (Price lname lquantity lprice : tail))

getIngredientCost' (name, quantity) ((Price lname lquantity lprice) : tail)
    | name == lname = (quantity / lquantity) * lprice
    | name /= lname = getIngredientCost (name, quantity) tail

-- Calculate how much it costs to buy all the ingredients of a recipe
recipeCost :: Recipe -> [Price] -> Double
recipeCost (Recipe name []) prices = 0
recipeCost (Recipe name ((rName, rQuantity) : rTail)) prices@((Price pName pQuantity pPrice) : pTail) = getRounded (recipeCost' (Recipe name ((rName, rQuantity) : rTail)) (Price pName pQuantity pPrice : pTail))

recipeCost' :: Recipe -> [Price] -> Double
recipeCost' (Recipe name ((rName, rQuantity) : rTail)) prices@((Price pName pQuantity pPrice) : pTail)
    | null rTail = getIngredientCost (rName, rQuantity) prices
    | otherwise = getIngredientCost (rName, rQuantity) prices + recipeCost (Recipe name rTail) prices

-- Given a list of how much you already have of each ingredient,
-- calculate how much of which ingredients are missing for a recipe
missingIngredients :: Recipe -> [(String, Double)] -> [(String, Double)]
missingIngredients recipe stock = filter removeZeroes $ map (stockIntoRounded . stockAbs) $ filter isMissing $ missingIngredients' recipe stock

missingIngredients' :: Recipe -> [(String, Double)] -> [(String, Double)]
missingIngredients' (Recipe name []) stock@((sName, sQuantity) : sTail) = stock
missingIngredients' recipe@(Recipe name ((rName, rQuantity) : rTail)) stock@((sName, sQuantity) : sTail) = missingIngredients' (Recipe name rTail) (missingIngredientsHelper (rName, rQuantity) stock)

missingIngredientsHelper :: (String, Double) -> [(String, Double)] -> [(String, Double)]
missingIngredientsHelper ingredient@(rName, rQuantity) [] = [(rName, -rQuantity)]
missingIngredientsHelper ingredient@(rName, rQuantity) stock@((sName, sQuantity) : sTail)
    | rName == sName = (sName, sQuantity - rQuantity) : sTail
    | otherwise = (sName, sQuantity) : missingIngredientsHelper ingredient sTail

isMissing :: (String, Double) -> Bool
isMissing (_, sQuantity) = sQuantity < 0

stockIntoRounded :: (String, Double) -> (String, Double)
stockIntoRounded (sName, sQuantity) = (sName, getRounded sQuantity)

stockAbs :: (String, Double) -> (String, Double)
stockAbs (sName, sQuantity) = (sName, -sQuantity)

-- Given a list of ingredients in your kitchen, calculate what you would
-- have left after making the given recipe. If there isn't enough of an
-- ingredient, the recipe cannot be made! You shouldn't change the amount
-- of ingredient in that case.
makeRecipe :: [(String, Double)] -> Recipe -> [(String, Double)]
makeRecipe stock recipe
    | null (missingIngredients recipe stock) = filter removeZeroes $ map stockIntoRounded $ missingIngredients' recipe stock
    | otherwise = filter removeZeroes $ map stockIntoRounded stock

removeZeroes :: (String, Double) -> Bool
removeZeroes (_, sQuantity) = sQuantity /= 0

-- Given a list of ingredients you already have, and a list of recipes,
-- make a shopping list showing how much of each ingredient you need
-- to buy, and its cost. Each ingredient mush appear in the shopping list
-- at most once (no duplicates!).
makeShoppingList :: [(String, Double)] -> [Recipe] -> [Price] -> [(String, Double, Double)]
makeShoppingList stock@((sName, sQuantity) : sTail) recipes@(recipe@(Recipe name ((rName, rQuantity) : rTail)) : rsTail) prices@((Price pName pQuantity pPrice) : pTail) = map (neededIntoRounded . neededWithCost prices . stockAbs) $ filter isMissing $ stockAfterRecipes recipes stock

stockAfterRecipes :: [Recipe] -> [(String, Double)] -> [(String, Double)]
stockAfterRecipes [] stock = stock
stockAfterRecipes recipes@(recipe@(Recipe name ((rName, rQuantity) : rTail)) : rsTail) stock@((sName, sQuantity) : sTail) = stockAfterRecipes rsTail (missingIngredients' recipe stock)

neededWithCost :: [Price] -> (String, Double) -> (String, Double, Double)
neededWithCost prices needed@(sName, sQuantity) = (sName, sQuantity, getIngredientCost needed prices)

neededIntoRounded :: (String, Double, Double) -> (String, Double, Double)
neededIntoRounded (nName, nQuantity, nPrice) = (nName, getRounded nQuantity, getRounded nPrice)
