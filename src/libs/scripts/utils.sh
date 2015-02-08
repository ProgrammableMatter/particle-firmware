# @author: Raoul Rubien 2011

# @param1: exitcode
# @param2: optional message

assertSuccess()
{
  cmd=$_
  state=$1
  messge=$2

  if [ $state != 0 ] ; then
    echo "WARNING: Last command [$cmd]"
    if [ -z "$message" ] ; then
        message="WARNING: Task aborted."
    else
        message="$message"
    fi
    echo -ne "\n\n$messge\n\n"
    exit 42
  fi
}

# @param1: exit code
# @param2: message to tell on erroneous exit code
warnExitcode()
{
  state=$1
  message=$2

  if [ "$state" != "0" ] ; then
    echo $message
  fi
}

# @param1: array with program names to be checked if install
checkInstalled()
{
  ret=0
  names=$@

  for name in ${names[@]} ; do
    # which $name > /dev/null 2>&1
    type $name > /dev/null 2>&1
    isNotInstalled=$?
    if [ "$isNotInstalled"  != "0" ] ; then
      ret=42
    fi
    warnExitcode $isNotInstalled "WARNING: Executable [$name] not installed."
  done

  return $ret
} 

