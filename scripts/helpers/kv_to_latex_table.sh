#!/bin/bash

function latexify {
    IFS=_ read name subscripts <<< "$1"
    if [ $subscripts ]
    then
        echo "\$\\text{$name}_{${subscripts//_/ }}\$"
    else
        echo "\$\\text{$name}\$"
    fi
}


echo "\\begin{tabular}{c | c}"
echo "    algorithm & rank \\\\"
echo "    \\hline"
while IFS=: read instance rank
do
    latex_instance=$(latexify $instance)
    echo "    $latex_instance & $rank \\\\"
done
echo "\\end{tabular}"
