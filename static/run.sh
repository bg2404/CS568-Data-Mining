if [[ $# -ne  5 ]]; then
	echo "./run.sh {file} {update file size} {update file} {min points} {epsilon}";
	exit;
fi
make;
make test;
./test $1 $3;
env time -v ./subclu $1 $2 $3 $4 $5;
str=$1;
python3 visualize.py ${str:0:(-4)}__$2.csv Subspace111.csv;
python3 silhouette.py ${str:0:(-4)}__$2.csv Subspace111.csv;
rm ${str:0:(-4)}__*.csv;
#make clean;
