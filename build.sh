red="\e[31m"
green="\e[32m"
yellow="\e[33m"
cyan="\e[36m"
normal="\e[0m"
debug=false
developing=false
release=false
profiling=false
isFlag=false
buildDirectory=""
options=""

# Iterate over each character in argument.
word="$1"
for i in $(seq 1 ${#word}); do
  ch=${word:i-1:1}
  case "$ch" in
    '-')
      # Our sequence starts from one.
      if [ "$i" == "1" ]; then
        isFlag=true
      fi
    ;;
    'd')
      debug=true
      if [ "$release" == "true" ] || [ "$developing" == "true" ]; then
        echo -e "$red-- Must describe either debug, release, or development building.$normal"
        exit 1
      fi

      options="$options -DCMAKE_BUILD_TYPE=Debug -DBUILD_DEBUGGING=ON"
      buildDirectory="debug"
    ;;
    'o') # Letter o, denoting optimized debugging.
      developing=true
      if [ "$debug" == "true" ] || [ "$release" == "true" ]; then
        echo -e "$red-- Must describe either debug, release, or development building.$normal"
        exit 1
      fi

      options="$options -DCMAKE_BUILD_TYPE=RelWithDebInfo -DBUILD_DEBUGGING=ON"
      buildDirectory="devel"
    ;;
    'r')
      release=true
      if [ "$debug" == "true" ] || [ "$developing" == "true" ]; then
        echo -e "$red-- Must describe either debug, release, or development building.$normal"
        exit 1
      fi

      options="$options -DCMAKE_BUILD_TYPE=Release"
      buildDirectory="release"
    ;;
    't') # Build tests.
      options="$options -DBUILD_TESTS=ON"
    ;;
    's') # Enable shared library building.
      options="$options -DBUILD_SHARED=ON"
    ;;
    'p') # Enable profiling.
      profiling=true
      options="$options -DBUILD_PROFILING=ON"
    ;;
    'b') # Build benchmarks.
      options="$options -DBUILD_BENCHMARKS=ON"
    ;;
    'q') # Enable quiet building.
      options="$options -DBUILD_QUIET=ON"
    ;;
  esac

  if [ "$isFlag" == "false" ]; then
    break
  fi
done

if [ "$options" == "" ]; then
  echo -e "$red-- No options specified, terminating.$options"
  exit 1
fi

if [ "$debug" == "false" ] && [ "$release" == "false" ] && [ "$developing" == "false" ]; then
  echo -e "$red-- No build type specified, defaulting to debug.$normal"
  options="$options -DCMAKE_BUILD_TYPE=Debug -DBUILD_DEBUGGING=ON"
  debug=true
fi

if [ "$release" == "true" ] && [ "$profiling" == "true" ]; then
  echo -e "$red-- Profiling cannot be enabled on a release build, please use a development build.$normal"
  exit 1
fi

if [ "$debug" == "true" ] && [ ! -d "$buildDirectory" ]; then
  echo -e "$yellow-- Debug directory missing, adding...$normal"
  mkdir debug
fi

if [ "$developing" == "true" ] && [ ! -d "$buildDirectory" ]; then
  echo -e "$yellow-- Development directory missing, adding...$normal"
  mkdir devel
fi

if [ "$release" == "true" ] && [ ! -d "$buildDirectory" ]; then
  echo -e "$yellow-- Release directory missing, adding...$normal"
  mkdir release
fi

pushd $buildDirectory > /dev/null
  cmake -G "Unix Makefiles" .. $options
  cmake --build . -- -j"$(($(nproc)/2+1))"
popd > /dev/null
