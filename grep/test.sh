TEST_FILES="1.txt 2.txt 3.txt"
COUNT=0

echo ""
echo "---Testing---"

echo ""
echo "***TEST 1 base***"
echo "=STD=:  " && grep "and" $TEST_FILES
echo "=S21=:  " && ./s21_grep "and" $TEST_FILES
diff <(grep "and" $TEST_FILES) <(./s21_grep "and" $TEST_FILES) && { echo "PASSED"; ((COUNT++)); } || echo "FAILED"

echo ""
echo "***TEST 2 -i***"
echo "=STD=:  " && grep -i "And" $TEST_FILES
echo "=S21=:  " && ./s21_grep -i "And" $TEST_FILES
diff <(grep -i "And" $TEST_FILES) <(./s21_grep -i "And" $TEST_FILES) && { echo "PASSED"; ((COUNT++)); } || echo "FAILED"

echo ""
echo "***TEST 3 -v***"
echo "=STD=:  " && grep -v "and" $TEST_FILES
echo "=S21=:  " && ./s21_grep -v "and" $TEST_FILES
diff <(grep -v "and" $TEST_FILES) <(./s21_grep -v "and" $TEST_FILES) && { echo "PASSED"; ((COUNT++)); } || echo "FAILED"

echo ""
echo "***TEST 4 -c***"
echo "=STD=:  " && grep -c "and" $TEST_FILES
echo "=S21=:  " && ./s21_grep -c "and" $TEST_FILES
diff <(grep -c "and" $TEST_FILES) <(./s21_grep -c "and" $TEST_FILES) && { echo "PASSED"; ((COUNT++)); } || echo "FAILED"

echo ""
echo "***TEST 5 -l***"
echo "=STD=:  " && grep -l "and" $TEST_FILES
echo "=S21=:  " && ./s21_grep -l "and" $TEST_FILES
diff <(grep -l "and" $TEST_FILES) <(./s21_grep -l "and" $TEST_FILES) && { echo "PASSED"; ((COUNT++)); } || echo "FAILED"

echo ""
echo "***TEST 6 -n***"
echo "=STD=:  " && grep -n "and" $TEST_FILES
echo "=S21=:  " && ./s21_grep -n "and" $TEST_FILES
diff <(grep -n "and" $TEST_FILES) <(./s21_grep -n "and" $TEST_FILES) && { echo "PASSED"; ((COUNT++)); } || echo "FAILED"

echo ""
echo "***TEST 7 -h***"
echo "=STD=:  " && grep -h "and" $TEST_FILES
echo "=S21=:  " && ./s21_grep -h "and" $TEST_FILES
diff <(grep -h "and" $TEST_FILES) <(./s21_grep -h "and" $TEST_FILES) && { echo "PASSED"; ((COUNT++)); } || echo "FAILED"

echo ""
echo "***TEST 8 -s***"
echo "=STD=:  " && grep -s "and" $TEST_FILES
echo "=S21=:  " && ./s21_grep -s "and" $TEST_FILES
diff <(grep -s "and" $TEST_FILES) <(./s21_grep -s "and" $TEST_FILES) && { echo "PASSED"; ((COUNT++)); } || echo "FAILED"

echo ""
echo "***TEST 9 -o***"
echo "=STD=:  " && grep -o "and" $TEST_FILES
echo "=S21=:  " && ./s21_grep -o "and" $TEST_FILES
diff <(grep -o "and" $TEST_FILES) <(./s21_grep -o "and" $TEST_FILES) && { echo "PASSED"; ((COUNT++)); } || echo "FAILED"

echo ""
echo "***TEST 10 -e***"
echo "=STD=:  " && grep -e "- a" $TEST_FILES
echo "=S21=:  " && ./s21_grep -e "- a" $TEST_FILES
diff <(grep -e "- a" $TEST_FILES) <(./s21_grep -e "- a" $TEST_FILES) && { echo "PASSED"; ((COUNT++)); } || echo "FAILED"

echo ""
echo "***TEST 11 -f***"
echo "=STD=:  " && grep -f $TEST_FILES
echo "=S21=:  " && ./s21_grep -f $TEST_FILES
diff <(grep -f $TEST_FILES) <(./s21_grep -f $TEST_FILES) && { echo "PASSED"; ((COUNT++)); } || echo "FAILED"

echo "PASSED: $COUNT / 11"
echo "Done"