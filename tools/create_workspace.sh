#!/bin/bash
if [ -z "$MPC_ROOT" ]
then
  echo "The MPC_ROOT environment variable is missing. You can download the 'The Makefile, Project, and Workspace Creator' from http://www.ociweb.com/products/mpc ."
  exit 1
fi
pushd `dirname $0`/..
$MPC_ROOT/mwc.pl -type make -include mpc -features boost=1 -value_template add_references=1 -static cck.mwc
if [ $? -eq 0 ]
then
  echo "Create workspace successful..."
fi
popd
exit

