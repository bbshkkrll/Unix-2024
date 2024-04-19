rm source.lck 2>>/dev/null
rm source 2>>/dev/null
rm result.txt 2>>/dev/null

touch source

pids=()

for i in {1..10}
do
    ./main source &
    pids+=($!)
done

sleep 15
echo ${pids[*]}
kill -2 ${pids[*]}

