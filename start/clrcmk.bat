@echo off
@echo clear cmake cache
if exist CMakeCache.txt (
	echo delete CMakeCache.txt
	del CMakeCache.txt
)

