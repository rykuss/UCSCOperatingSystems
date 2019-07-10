archive=~/CMPS111-Lab3.tar
echo "Creating $archive.gz"
rm $archive 2>/dev/null
cd ../..

tar cvf $archive \
	`diff -u -r ../pintos ../.pintos | lsdiff | sed 's/\/home\/pintos\/pintos\///' | sed '/src\/threads\/nbproject/d'` 2>/dev/null

tar rvf $archive \
	`diff -r ../.pintos ../pintos | grep Only | sed 's/: /\//' | sed '/\/\.pintos/d' | sed 's/Only in //' | sed 's/\/home\/pintos\/pintos\///' | sed '/\/.git/d' | sed '/^>/d'`

gzip $archive
