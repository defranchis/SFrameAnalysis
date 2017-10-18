
#!bin/bash

oldstring='extBatchSubmission'
newstring='JERbatchSubmission'
echo "old string: " $oldstring
echo "new string: " $newstring
grep -rl $oldstring ./*.py | xargs sed -i s~$oldstring~$newstring~g