{-# OPTIONS_GHC -Wno-unrecognised-pragmas #-}
{-# HLINT ignore "Use bimap" #-}
module PE2 where

-- PE2: Dungeon Crawler
-- Dungeon map is :: Tree Chamber [Encounter]
-- Each encounter is either a fight or a treasure
-- Fights deal you damage (reduce HP) but enemies drop some gold (add
-- gold)
-- Tresures just give gold, or potions (which give hp)
-- Nodes hold encounters, when you visit a node you go through all of them in order
-- You start with a certain amount of HP and 0 gold.
-- You lose HP and accumulate gold as you descend the tree and go through encounters

-- Polymorphic tree structure
data Tree a b = EmptyTree | Leaf a b | Node a b [Tree a b] deriving (Show, Eq)

-- Every location in the tree is of some Chamber type.
data Chamber = Cavern |
               NarrowPassage |
               UndergroundRiver |
               SlipperyRocks deriving (Show, Eq)

-- An enemy has a name, an amount of damage that it deals
-- and an amount of gold that it drops (in that order).
data Enemy = Enemy String Integer Integer deriving (Show, Eq)

-- Gold n gives n amount of gold
-- Potion n heals n hp
data Loot = Gold Integer | Potion Integer deriving (Show, Eq)

-- An encounter is either a Fight with an Enemy, or a treasure where
-- you find Loot
data Encounter = Fight Enemy | Treasure Loot deriving (Show, Eq)

-- This is a type synonym for how we will represents our dungeons
type Dungeon = Tree Chamber [Encounter]

-- First argument is starting HP
-- Second argument is the dungeon map
-- Third argument is the path (each integer in the list shows what child
-- you descend into)
-- Calculate how much HP you have left and how much gold you've
-- accumulated after traversing the given path
traversePath :: Integer -> Dungeon -> [Int] -> (Integer, Integer)
traversePath hp = traverseGold hp 0

traverseGold :: Integer -> Integer -> Dungeon ->[Int] -> (Integer, Integer)
traverseGold hp gold EmptyTree [] = (hp, gold)
traverseGold hp gold (Leaf _ []) [] = (hp, gold)
traverseGold hp gold (Leaf chamber (encounter : eTail)) [] = case encounter of Fight (Enemy _ attack loot) -> traverseGold (hp-attack) (gold+loot) (Leaf chamber eTail) []
                                                                               Treasure (Gold loot) -> traverseGold hp (gold+loot) (Leaf chamber eTail) []
                                                                               Treasure (Potion heal) -> traverseGold (hp+heal) gold (Leaf chamber eTail) []
traverseGold hp gold (Node chamber [] (curChild : cTail)) (numChild : nTail)
    | numChild == 0 = traverseGold hp gold curChild nTail
    | otherwise = traverseGold hp gold (Node chamber [] cTail) ((numChild-1) : nTail)
traverseGold hp gold (Node chamber (encounter : eTail) children) path = case encounter of Fight (Enemy _ attack loot) -> traverseGold (hp-attack) (gold+loot) (Node chamber eTail children) path
                                                                                          Treasure (Gold loot) -> traverseGold hp (gold+loot) (Node chamber eTail children) path
                                                                                          Treasure (Potion heal) -> traverseGold (hp+heal) gold (Node chamber eTail children) path

-- First argument is starting HP
-- Second argument is dungeon map
-- Find which path down the tree yields the most gold for you
-- You cannot turn back, i.e. you'll find a non-branching path
-- You do not need to reach the bottom of the tree
-- Return how much gold you've accumulated
findMaximumGain :: Integer -> Dungeon -> Integer
findMaximumGain _ EmptyTree = 0
findMaximumGain hp (Leaf _ []) = 0
findMaximumGain hp (Leaf chamber (encounter : eTail)) = case encounter of Fight (Enemy _ attack loot) -> if (hp>attack) && (findMaximumGain (hp-attack) (Leaf chamber eTail)>(-1)) then loot + findMaximumGain (hp-attack) (Leaf chamber eTail) else -1
                                                                          Treasure (Gold loot) -> if findMaximumGain hp (Leaf chamber eTail)>(-1) then loot + findMaximumGain hp  (Leaf chamber eTail) else -1
                                                                          Treasure (Potion heal) -> findMaximumGain (hp+heal)  (Leaf chamber eTail)
findMaximumGain hp (Node chamber [] []) = 0
findMaximumGain hp (Node chamber [] (child : cTail)) = max (findMaximumGain hp child) (findMaximumGain hp (Node chamber [] cTail))
findMaximumGain hp (Node chamber (encounter : eTail) children) = case encounter of Fight (Enemy _ attack loot) -> if (hp>attack) && (findMaximumGain (hp-attack) nextEncounter>(-1)) then loot + findMaximumGain (hp-attack) nextEncounter else -1
                                                                                   Treasure (Gold loot) -> if findMaximumGain hp nextEncounter>(-1) then loot + findMaximumGain hp nextEncounter else -1
                                                                                   Treasure (Potion heal) -> findMaximumGain (hp+heal) nextEncounter
                                                                                   where nextEncounter = Node chamber eTail children

-- First argument is starting HP
-- Second argument is the dungeon map
-- Remove paths that you cannot go thorugh with your starting HP. (By
-- removing nodes from tree).
-- Some internal nodes may become leafs during this process, make the
-- necessary changes in such a case.
findViablePaths :: Integer -> Dungeon -> Dungeon
findViablePaths _ EmptyTree = EmptyTree
findViablePaths hp (Leaf chamber []) = Leaf chamber []
findViablePaths hp (Leaf chamber encounters) = if viableChecker hp (Leaf chamber encounters) then Leaf chamber encounters else EmptyTree
findViablePaths hp (Node chamber [] children) | null newChildren = Leaf chamber []
                                              | otherwise = Node chamber [] (map (findViablePaths hp) newChildren)
                                              where newChildren = filter (viableChecker hp) children
findViablePaths hp (Node chamber encounters children) = if viableChecker hp (Node chamber encounters children) then if null newChildren then Leaf chamber encounters
                                                                                                                    else Node chamber encounters (map (findViablePaths newHp) newChildren)
                                                                                                               else EmptyTree
                                                                                                               where newHp = hpAfterEncounters hp encounters
                                                                                                                     newChildren = filter (viableChecker newHp) children

hpAfterEncounters :: Integer -> [Encounter] -> Integer
hpAfterEncounters hp [] = hp
hpAfterEncounters hp (encounter : eTail) = case encounter of Fight (Enemy _ attack loot) -> hpAfterEncounters (hp-attack) eTail
                                                             Treasure (Gold loot) -> hpAfterEncounters hp eTail
                                                             Treasure (Potion heal) -> hpAfterEncounters (hp+heal) eTail

viableChecker :: Integer -> Dungeon -> Bool
viableChecker hp EmptyTree = True
viableChecker hp (Leaf _ []) = True
viableChecker hp (Leaf chamber (encounter : eTail)) = let nextEncounter = Leaf chamber eTail
                                                       in case encounter of Fight (Enemy _ attack loot) -> (hp>attack) && viableChecker (hp-attack) nextEncounter
                                                                            Treasure (Gold loot) -> viableChecker hp nextEncounter
                                                                            Treasure (Potion heal) -> viableChecker (hp+heal) nextEncounter
viableChecker hp (Node chamber [] children) = True
viableChecker hp (Node chamber (encounter : eTail) children) = let nextEncounter = Node chamber eTail children
                                                                in case encounter of Fight (Enemy _ attack loot) -> (hp>attack) && viableChecker (hp-attack) nextEncounter
                                                                                     Treasure (Gold loot) -> viableChecker hp nextEncounter
                                                                                     Treasure (Potion heal) -> viableChecker (hp+heal) nextEncounter

-- First argument is starting HP
-- Second Argument is dungeon map
-- Find, among the viable paths in the tree (so the nodes you cannot visit is already removed) the two most distant nodes, i.e. the two nodes that are furthest awat from each other.
mostDistantPair :: Integer -> Dungeon -> (Integer, Dungeon)
mostDistantPair hp dungeon = possibleDistantPair (findViablePaths hp dungeon)

possibleDistantPair :: Dungeon -> (Integer, Dungeon)
possibleDistantPair EmptyTree = (0, EmptyTree)
possibleDistantPair (Leaf chamber encounters) = (0, Leaf chamber encounters)
possibleDistantPair dungeon@(Node chamber encounters [child]) = let belowDepth = highestDepth dungeon
                                                                in maxTuple (1+fst belowDepth, Node chamber encounters [snd belowDepth]) $ possibleDistantPair child
possibleDistantPair dungeon@(Node chamber encounters children) = let top2 = top2HighestDepth $ map highestDepthWithSelf children
                                                                     firstTuple = fst top2
                                                                     secondTuple = snd top2
                                                                  in maxTuple (2+fst firstTuple+fst secondTuple, Node chamber encounters [snd firstTuple,snd secondTuple]) (moveOnToChildren dungeon)

top2HighestDepth :: [(Integer, Dungeon)] -> ((Integer, Dungeon),(Integer, Dungeon))
top2HighestDepth list = properOrder list (firstTuple, secondTuple)
                        where firstTuple = maxTupleInList list
                              secondTuple = maxTupleInList (filter (/= firstTuple) list)

properOrder :: [(Integer, Dungeon)] -> ((Integer, Dungeon),(Integer, Dungeon)) -> ((Integer, Dungeon),(Integer, Dungeon))
properOrder (tuple:tTail) (firstTuple, secondTuple) | tuple == firstTuple = (firstTuple, secondTuple)
                                                    | tuple == secondTuple = (secondTuple, firstTuple)
                                                    | otherwise = properOrder tTail (firstTuple, secondTuple)

moveOnToChildren :: Dungeon -> (Integer, Dungeon)
moveOnToChildren (Node chamber encounters []) = (0, EmptyTree)
moveOnToChildren (Node chamber encounters (child:cTail)) = maxTuple (possibleDistantPair child) (moveOnToChildren (Node chamber encounters cTail))

highestDepthWithSelf :: Dungeon -> (Integer, Dungeon)
highestDepthWithSelf dungeon@(Leaf chamber encounters) = (0,dungeon)
highestDepthWithSelf dungeon@(Node chamber encounters children) = let belowMaxDepth = highestDepth dungeon
                                                                  in (1+fst belowMaxDepth, Node chamber encounters [snd belowMaxDepth])

highestDepth :: Dungeon -> (Integer, Dungeon)
highestDepth EmptyTree = (0, EmptyTree)
highestDepth (Leaf chamber encounters) = (0, Leaf chamber encounters)
highestDepth (Node chamber encounters []) = (0, EmptyTree)
highestDepth (Node chamber encounters (child:cTail)) = let deepestFirstChild = case child of childLeaf@(Leaf childChamber childEncounters) -> (0, childLeaf)
                                                                                             childNode@(Node childChamber childEncounters childChildren) -> let highestGrandChild = highestDepth childNode
                                                                                                                                                             in (1+fst highestGrandChild, Node childChamber childEncounters [snd highestGrandChild])
                                                       in maxTuple deepestFirstChild $ highestDepth (Node chamber encounters cTail)

maxTuple :: (Integer, Dungeon) -> (Integer, Dungeon) -> (Integer, Dungeon)
maxTuple (a, b) (c, d) = if a>=c then (a, b) else (c, d)

maxTupleInList :: [(Integer, Dungeon)] -> (Integer, Dungeon)
maxTupleInList = foldr maxTuple (0, EmptyTree)

-- Find the subtree that has the highest total gold/damage ratio
-- Simply divide the total gold in the subtree by the total damage
-- in the subtree. You only take whole subtrees (i.e you can take a new
-- node as the root of your subtree, but you cannot remove nodes
-- below it). Note that the answer may be the whole tree.
mostEfficientSubtree :: Dungeon -> Dungeon
mostEfficientSubtree EmptyTree = EmptyTree
mostEfficientSubtree dungeon@(Leaf chamber encounters) = dungeon
mostEfficientSubtree dungeon@(Node chamber encounters children) = let currentEfficiency = calculateEfficiency dungeon
                                                                      highestChild = mostEfficientInList children
                                                                      childEfficiency = calculateEfficiency highestChild
                                                                   in if currentEfficiency >= childEfficiency then dungeon else highestChild

mostEfficientInList :: [Dungeon] -> Dungeon
mostEfficientInList [] = EmptyTree
mostEfficientInList (node:nTail) | calculateEfficiency (mostEfficientSubtree node) >= calculateEfficiency (mostEfficientInList nTail) = mostEfficientSubtree node
                                 | otherwise = mostEfficientInList nTail

calculateEfficiency :: Dungeon -> Double
calculateEfficiency EmptyTree = 0
calculateEfficiency dungeon = let (gold,damage) = traverseTree dungeon
                               in if damage <= 0 then 1/0
                                  else fromInteger gold / fromInteger damage

traverseTree :: Dungeon -> (Integer, Integer)
traverseTree EmptyTree = (0, 0)
traverseTree (Leaf chamber [] ) = (0, 0)
traverseTree (Leaf chamber encounters) = applyEncounters encounters
traverseTree (Node chamber _ []) = (0,0)
traverseTree (Node chamber [] (child:cTail)) = let childResult = traverseTree child
                                                   cTailResult = traverseTree (Node chamber [] cTail)
                                                in (fst childResult + fst cTailResult, snd childResult + snd cTailResult)
traverseTree (Node chamber encounters (child:cTail)) = let currentResult = applyEncounters encounters
                                                           childResult = traverseTree child
                                                           cTailResult = traverseTree (Node chamber [] cTail)
                                                        in (fst currentResult + fst childResult + fst cTailResult, snd currentResult + snd childResult + snd cTailResult)

applyEncounters :: [Encounter] -> (Integer, Integer)
applyEncounters [] = (0, 0)
applyEncounters (encounter : eTail) = let encounterRest = applyEncounters eTail
                                          goldRest = fst encounterRest
                                          damageRest = snd encounterRest
                                       in case encounter of Fight (Enemy _ attack loot) -> (loot+goldRest, attack+damageRest)
                                                            Treasure (Gold loot) -> (loot+goldRest, damageRest)
                                                            Treasure (Potion heal) -> (goldRest, damageRest-heal)