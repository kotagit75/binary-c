rm -r cell/*
make clean
./tool/sample_putA_comp 10
./tool/sample_exit0_comp 5
cp -r ./sample_struct/1 cell/1
cp -r ./sample_struct/1 cell/2