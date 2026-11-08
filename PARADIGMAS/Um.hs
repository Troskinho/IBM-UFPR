module Um where

--calcular aumento de salario
salario :: Float -> Float
salario x = x + (x * 25) / 100

--calcular media ponderada
mediaP :: Float -> Float -> Float -> Float -> Float -> Float -> Float
mediaP x a y b z c = (x * a + y  * b + z * c) / peso
       where 
            peso = a + b + c

--converter celsius para fahrenheint
celsius :: Float -> Float
celsius c  = c * (9 / 5) + 32

--calcular ano de nascimento em 2032
idade :: Int -> Int
idade a = 2032 - a

--transformar horas e minutos em apenas minutos
hora :: Int -> Int -> Int
hora a b = (a * 60) + b
