addSr :: String -> String
addSr nome = "Sr. " ++ nome

addSra :: String -> String
addSra nome = "Sra. " ++ nome

addSrta :: String -> String
addSrta nome = "Srta. " ++ nome

mapearTratamento :: [String] -> (String -> String) -> [String]
mapearTratamento [] _ = []
mapearTratamento (x:xs) f = f x : mapearTratamento xs f


ehPositivo :: Float -> Bool
ehPositivo x = x > 0

ehNegativo :: Float -> Bool
ehNegativo x = x < 0

ehZero :: Float -> Bool
ehZero x = x == 0

filtrarValores :: [Float] -> (Float -> Bool) -> [Float]
filtrarValores [] _ = []
filtrarValores (x:xs) f
    | f x       = x : filtrarValores xs f
    | otherwise = filtrarValores xs f





-- Arquivo: Main.hs[cite: 4]
module Main where
import Tratamento
import Validacao

main :: IO ()
main = do
    let nomes = ["Joao", "Maria", "Ana"]
    let valores = [10.5, -3.2, 0.0, 7.1, -1.0]

    putStrLn "--- Teste do Modulo Tratamento ---"
    print $ mapearTratamento nomes addSr
    print $ mapearTratamento nomes addSra
    print $ mapearTratamento nomes addSrta

    putStrLn "\n--- Teste do Modulo Validacao ---"
    print $ filtrarValores valores ehPositivo
    print $ filtrarValores valores ehNegativo
    print $ filtrarValores valores ehZero
