echo Executgin commands from .gdbinit...
handle SIGUSR1 "nostop" "noprint" "pass"

file out/test_apis
#file out/test_tasks

define run_test
  run -c -v -n One -r
end
alias -- rt = run_test

echo done.\n
