# CMake generated Testfile for 
# Source directory: /Users/friend/Library/Mobile Documents/com~apple~CloudDocs/Documents/Dev/Super_VelocityCurve
# Build directory: /Users/friend/Library/Mobile Documents/com~apple~CloudDocs/Documents/Dev/Super_VelocityCurve/build-rwd
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test([=[EngineTests]=] "/Users/friend/Library/Mobile Documents/com~apple~CloudDocs/Documents/Dev/Super_VelocityCurve/build-rwd/SuperVelocityCurveTests_artefacts/RelWithDebInfo/SuperVelocityCurveTests")
set_tests_properties([=[EngineTests]=] PROPERTIES  _BACKTRACE_TRIPLES "/Users/friend/Library/Mobile Documents/com~apple~CloudDocs/Documents/Dev/Super_VelocityCurve/CMakeLists.txt;155;add_test;/Users/friend/Library/Mobile Documents/com~apple~CloudDocs/Documents/Dev/Super_VelocityCurve/CMakeLists.txt;0;")
subdirs("JUCE")
subdirs("third_party/clap-juce-extensions")
