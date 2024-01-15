module ColorPrint where

import Data.List (intercalate)
import Data.List.Split (splitOn)
import System.Console.ANSI

-- Format string with color and optional bold
getColor :: String -> Color -> Bool -> String
getColor text color bold =
  let coloredText = replace (setSGRCode [Reset]) (setSGRCode [Reset] ++ setSGRCode [SetColor Foreground Vivid color]) text
      boldCode = if bold then setSGRCode [SetConsoleIntensity BoldIntensity] else ""
   in setSGRCode [SetColor Foreground Vivid color] ++ boldCode ++ coloredText ++ setSGRCode [Reset]

-- Print text with specified color
printColor :: String -> Color -> IO ()
printColor text color = putStrLn $ getColor text color False

-- Print bold text with optional color
printBold :: String -> Color -> IO ()
printBold text color = putStrLn $ getColor text color True

-- Print error message with red color
printError :: String -> IO ()
printError message =
  let (indent, text) = splitLeadingWhitespace message
   in putStrLn $ indent ++ getColor ("Error: " ++ text) Red True

-- Print warning message with yellow color
printWarn :: String -> IO ()
printWarn message =
  let (indent, text) = splitLeadingWhitespace message
   in putStrLn $ indent ++ getColor ("Warning: " ++ text) Yellow True

-- Split a string into the leading whitespace and the rest of the string
splitLeadingWhitespace :: String -> (String, String)
splitLeadingWhitespace message =
  let indentSize = length $ takeWhile (== ' ') message
   in (take indentSize message, drop indentSize message)

-- Helper function to replace a substring
replace :: String -> String -> String -> String
replace old new = intercalate new . splitOn old
