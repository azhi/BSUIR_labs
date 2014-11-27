# lab1

Chiu fuzzy cluster estimation

## Usage

`lein run <filename> <ra> <distance_func_name>`

WHERE

* `<filename>` - path to a file with input data.
Each row represents an array of object attributes.
If the last attribute contains non-numeric characters, it is considered to be cluster name.
* `<ra>` - radius defining a neighborhood.
* `<distance_func_name>` - either 'euclid' or 'manhattan'.

## Output

Comma-separated list of cluster core's indexes in input file (starts with 0)

## Examples

`lein run resources/flowers.txt 1.5 euclid`

`lein run resources/butterfly.txt 7 manhattan`

`lein run resources/glass.txt 0.9 euclid`
