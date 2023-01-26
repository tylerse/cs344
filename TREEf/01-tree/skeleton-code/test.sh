DIR=/class/cs344/assignments/01-tree/test-dirs/$1
echo "running tests in $DIR"
for OPTS in {p,}{u,}{g,}{s,}{a,}{d,}{U,r,t,}
  do
    if ! cmp  <(tree "${OPTS:+"-$OPTS"}" -- $DIR)\
              <(./tree "${OPTS:+"-$OPTS"}" -- $DIR)
      then
        echo different
      else
         echo same
    fi
  done

