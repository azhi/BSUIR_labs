(ns lab1.core
  (:require [clojure.string :as string]
            [clojure-csv.core :as csv]
            [clojure.math.numeric-tower :as math])
  (:gen-class))

(deftype ClObject [attributes potential])

(defn parse
  "Parse input file into list of object attributes"
  [in_filename]
  (let [raw_data (csv/parse-csv (slurp in_filename))]
    (map
      (fn [attributes_vector]
        (let [stripped_attributes_vector
          (if (= (re-find #"\D+" (last attributes_vector)) nil)
            ;; last attribute has just numbers
            attributes_vector
            ;; last 'attribute' is name of class, omit it
            (butlast attributes_vector))]
          (map #(Float/parseFloat %) stripped_attributes_vector)))
      raw_data)))

(defn adjust_potentials
  "Low the potentials near new-found core"
  [objects new_core betta distance_func]
  (map
    (fn [object]
      (new ClObject
        (.attributes object)
        (-
          (.potential object)
          (*
            (.potential new_core)
            (let [distance (distance_func (.attributes object) (.attributes new_core))]
              (Math/exp (* (- betta) (math/expt distance 2))))))))
    objects))

(defn calc_potential
  "Calculate measure of potential to become cluster core"
  [object other_object alpha distance_func]
  (let [distance (distance_func object other_object)]
    (Math/exp (* (- alpha) (math/expt distance 2)))))

(defn prepare_input_objects
  "Returns a map of object and it's initial potential"
  [input_objects alpha distance_func]
  (map
    (fn [object_attributes]
      (new ClObject
        object_attributes
        (reduce +
          (map
            (fn [other_object_attributes]
              (calc_potential object_attributes other_object_attributes alpha distance_func))
            input_objects))))
    input_objects))

(defn estimate_cores
  "Find cluster cores positioning based on Chiu cluster estimation. Return indexes in input_objects array"
  [input_objects ra distance_func]
  (let [alpha (/ 4 (math/expt ra 2))
        rb (* 1.5 ra)
        betta (/ 4 (math/expt rb 2))
        eps_high 0.5
        eps_low 0.15]
    (loop [objects (prepare_input_objects input_objects alpha distance_func)
           first_core_potential nil
           found_cores []]
      (let [
             [core_candidate_index core_candidate]
               (apply max-key #(.potential (second %)) (map-indexed vector objects))
           ]
        (if (= first_core_potential nil)
          ;; then
          (recur
            (adjust_potentials objects core_candidate betta distance_func)
            (.potential core_candidate)
            (cons core_candidate_index found_cores))
          ;; else
          (if (> (.potential core_candidate) (* eps_high first_core_potential))
            ;; then
            (recur
              (adjust_potentials objects core_candidate betta distance_func)
              first_core_potential
              (cons core_candidate_index found_cores))
            ;; else
            (if (> (.potential core_candidate) (* eps_low first_core_potential))
              ;; then
              (let [dmin (apply min (map #(distance_func (.attributes core_candidate) (.attributes (nth objects %))) found_cores))]
                (if (>= (+ (/ dmin ra) (/ (.potential core_candidate) first_core_potential)) 1)
                  ;; then
                  (recur
                    (adjust_potentials objects core_candidate betta distance_func)
                    first_core_potential
                    (cons core_candidate_index found_cores))
                  ;; else
                  (recur
                    (assoc (apply vector objects) core_candidate_index (new ClObject (.attributes core_candidate) 0))
                    first_core_potential
                    found_cores)))
              ;; else
              found_cores)))))))

(defn euclid_distance
  [object1 object2]
  (math/sqrt (reduce + (map #(math/expt (- %1 %2) 2) object1 object2))))

(defn manhattan_distance
  [object1 object2]
  (reduce + (map #(math/abs (- %1 %2)) object1 object2)))

(defn -main
  [in_filename ra distance_func_name]
  (print (str (string/join ","
    (estimate_cores
      (parse in_filename)
      (Float/parseFloat ra)
      (case distance_func_name
        "euclid" euclid_distance
        "manhattan" manhattan_distance))) "\n")))
