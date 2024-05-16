(ns othello.othello
  (:require [clojure.tools.cli :refer [parse-opts]]
            [clojure.string :as str])
  (:gen-class))

(def min-board-size 4)
(def max-board-size 10)
(def version "0.1.0")

(def cli-options
  [["-a" "--autoplay" "Enable autoplay mode"]
   ["-d" "--default" "Play with default settings"]
   ["-l" "--log" "Show log after a game"]
   ["-n" "--no-helpers" "Hide disk placement hints"]
   ["-t" "--test" "Enable test mode"]
   ["-v" "--version" "Print version and exit"]
   ["-h" "--help" "Print help and exit"]])

(defn usage [options-summary]
  (->> ["Usage: othello_clojure [options] [size]"
        ""
        "Options:"
        options-summary]
       (str/join \newline)))

(defn error-msg [errors]
  (str "The following errors occurred while parsing your command:\n\n"
       (str/join \newline errors)))

(defn validate-args [args]
  (let [{:keys [options arguments errors summary]} (parse-opts args cli-options)]
    (cond
      (:help options) (println (usage summary))
      (:version options) (println (str "Othello Clojure " version))
      errors (println (error-msg errors))
      :else {:options options :size (first arguments)})))

(defn -main [& args]
  (let [{:keys [options size]} (validate-args args)]
    (when size
      (let [size (Integer. size)]
        (when (or (< size min-board-size) (> size max-board-size))
          (println (str "Size must be between " min-board-size " and " max-board-size)))))

    (println "Options:" options)
    (println "Board size:" size)))
