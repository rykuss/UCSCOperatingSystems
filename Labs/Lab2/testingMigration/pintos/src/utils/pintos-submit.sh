archive=~/CMPS111-Lab2.tar
echo "Creating $archive.gz"
rm $archive* 2>/dev/null
cd ../../..
BASE=`pwd`

tar cvf $archive \
	`diff -u -r pintos .pintos | lsdiff | sed 's/$BASE//' | sed '/nbproject/d'` 2>/dev/null

tar rvf $archive \
	`diff -r .pintos pintos | grep Only | sed 's/: /\//' | sed '/\/\.pintos/d' | sed 's/Only in //' | sed 's/$BASE//' | sed '/^>/d'`

gzip $archive
