if [ $# -ne 1 ]; then 
    echo "Usage bash sentence-counter.sh <char>"
    exit 1
fi

good_sentences=0

while read line
do 
    if ! echo $line | grep -q ",și"; then                                   # Doesn't contain ',și'.
        if echo $line | grep -q "$1"; then                                  # Contains <char>.
            if echo $line | grep -q "^[A-Z][a-zA-Z0-9 ,.!?]*[.!?]$"; then   # Satisfies conditions.
                good_sentences=`expr $good_sentences + 1`
            fi
        fi
    fi
done

echo "$good_sentences"
