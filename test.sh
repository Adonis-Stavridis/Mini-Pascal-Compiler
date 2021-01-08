#!/bin/bash

APP=bin/scalpa
VALGRIND=valgrind
CSPIM=spim

SCALPA_FILES=test/*.p

OUT_DIR=out/
MIPS_DIR=mips/
LOGS_DIR=logs/

if [ -f "$APP" ]; then
	echo -e "\e[1m\e[34mStarting tests\e[0m\e[39m"
	mkdir -p $OUT_DIR
	mkdir -p $OUT_DIR$LOGS_DIR

	for file in $SCALPA_FILES
	do
		filepath=${file%.*}
		filename=${filepath##*/}
		echo -e "\e[1m\e[33mTesting $file\e[0m\e[39m"
		echo -e "============= Scalpa Output =============" > \
			$OUT_DIR$LOGS_DIR$filename.log
		$APP $file -o $OUT_DIR$MIPS_DIR$filename.s -tos >> \
			$OUT_DIR$LOGS_DIR$filename.log 2>&1
		echo -e >> $OUT_DIR$LOGS_DIR$filename.log
		if [ -f "$OUT_DIR$MIPS_DIR$filename.s" ]; then
			echo -e "============= Spim Output =============" >> \
				$OUT_DIR$LOGS_DIR$filename.log
			$CSPIM -f $OUT_DIR$MIPS_DIR$filename.s >> \
				$OUT_DIR$LOGS_DIR$filename.log 2>&1
		fi
		cat $OUT_DIR$LOGS_DIR$filename.log
		echo -e
	done
else
	echo -e "\e[1m\e[31mNo executable $APP\e[0m\e[39m"
fi
