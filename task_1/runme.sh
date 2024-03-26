rm ./*_file 2>>/dev/null
rm ./*gz 2>>/dev/null

fallocate -l 4194305 ./a_file

make myprogram 1>>/dev/null

./init ./a_file

./myprogram -ia_file -o b_file

gzip -k a_file
gzip -k b_file
gzip -cd b_file.gz | ./myprogram -o c_file

./myprogram -ia_file -o d_file -s100

stat a_file | head -2 > result.txt
echo >> result.txt 
stat a_file.gz | head -2 >> result.txt
echo >> result.txt
stat b_file | head -2 >> result.txt
echo >> result.txt
stat b_file.gz | head -2 >> result.txt
echo >> result.txt
stat c_file | head -2 >> result.txt
echo >> result.txt
stat d_file | head -2 >> result.txt

cat result.txt

make clean 1>>/dev/null 
