(defproject lab1 "0.1.0-SNAPSHOT"
  :description "fuzzy cluster estimation"
  :url ""
  :license {:name "Eclipse Public License"
            :url "http://www.eclipse.org/legal/epl-v10.html"}
  :dependencies [
                  [org.clojure/clojure "1.6.0"]
                  [clojure-csv/clojure-csv "2.0.1"]
                  [org.clojure/math.numeric-tower "0.0.4"]
                ]
  :main ^:skip-aot lab1.core
  :target-path "target/%s"
  :profiles {:uberjar {:aot :all}})
