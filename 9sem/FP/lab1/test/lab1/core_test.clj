(ns lab1.core-test
  (:require [clojure.test :refer :all]
            [lab1.core :refer :all]))

(declare ^:dynamic distance_func)
(deftest distance_test
  ;; function under test must be defined as 'distance_func'
  (testing "Basic distance properties"
    (is (>= (distance_func [1, 1, 1] [2, 2, 2]) 0)) ;; non-negative
    (is (= (distance_func [1, 2, 3] [1, 2, 3]) 0)) ;; identity of indiscernibles
    (is (= (distance_func [1, 2, 3] [4, 5, 6]) (distance_func [4, 5, 6] [1, 2, 3]))) ;; symmetry
    (is (<= (distance_func [1, 2, 3] [4, 5, 6])
            (+ (distance_func [1, 2, 3] [7, 8, 9]) (distance_func [7, 8, 9] [4, 5, 6])))) ;; triangle inequality
))

(deftest euclid_distance_test
  (binding [distance_func euclid_distance]
    (distance_test))
  (testing "Some simple distances"
    (is (= (euclid_distance [1, 1] [2, 1]) 1))
    (is (= (euclid_distance [0, 0] [3, 4]) 5))))

(deftest manhattan_distance_test
  (binding [distance_func manhattan_distance]
    (distance_test))
  (testing "Some simple distances"
    (is (= (manhattan_distance [1, 1] [2, 1]) 1))
    (is (= (manhattan_distance [0, 0] [3, 4]) 7))))

(defn test-ns-hook []
  (euclid_distance_test)
  (manhattan_distance_test))
