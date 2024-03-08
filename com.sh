git add -A
if [ $# != 1 ];then
  git commit -m "add"
else
  git commit -m "$1"    
fi
git push origin master
