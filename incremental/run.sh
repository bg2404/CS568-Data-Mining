if [[ $# -ne  4 ]]; then
	echo "./run.sh {file} {update file} {min points} {epsilon}";
	exit;
fi
make;
env time -v ./subclu $1 $2 $3 $4;
str=$1;
python3 visualize.py ${str:0:(-4)}__updated.csv Subspace111.csv;
python3 silhouette.py ${str:0:(-4)}__updated.csv Subspace111.csv;
make clean;
