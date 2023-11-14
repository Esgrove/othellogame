module Main where

import ColorPrint
import Data.Semigroup ()
import Data.Version (showVersion)
import Options.Applicative
-- Import version from Cabal package
import Paths_othello (version)
import System.Console.ANSI
import Text.Printf

minBoardSize :: Int
minBoardSize = 4

maxBoardSize :: Int
maxBoardSize = 10

defaultBoardSize :: Int
defaultBoardSize = 8

data Options = Options
  { size :: Maybe Int,
    autoplay :: Bool,
    defaultSet :: Bool,
    showLog :: Bool,
    noHelpers :: Bool,
    test :: Bool
  }

sizeHelpMessage :: String
sizeHelpMessage = printf "Optional board size (%d..%d)" minBoardSize maxBoardSize

options :: Parser Options
options =
  Options
    <$> optional
      ( argument
          auto
          (metavar "SIZE" <> help sizeHelpMessage)
      )
    <*> switch (long "autoplay" <> short 'a' <> help "Enable autoplay mode")
    <*> switch (long "default" <> short 'd' <> help "Play with default settings")
    <*> switch (long "log" <> short 'l' <> help "Show log after a game")
    <*> switch (long "no-helpers" <> short 'n' <> help "Hide disk placement hints")
    <*> switch (long "test" <> short 't' <> help "Enable test mode")

main :: IO ()
main = do
  printBold "OTHELLO GAME - Haskell" System.Console.ANSI.Green
  execParser opts >>= runWithOptions
  where
    opts =
      info
        (helper <*> versionOption <*> options)
        ( fullDesc
            <> progDesc "A simple Othello CLI game implementation."
        )
    versionOption =
      infoOption
        (showVersion version)
        (long "version" <> short 'v' <> help "Print version and exit")

runWithOptions :: Options -> IO ()
runWithOptions opts = do
  putStrLn "Parsed options:"
  putStrLn $ "Size: " ++ show (size opts)
  putStrLn $ "Autoplay: " ++ show (autoplay opts)
  putStrLn $ "Default settings: " ++ show (defaultSet opts)
  putStrLn $ "Log: " ++ show (showLog opts)
  putStrLn $ "No helpers: " ++ show (noHelpers opts)
  putStrLn $ "Test mode: " ++ show (test opts)
