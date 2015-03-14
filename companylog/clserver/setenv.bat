@echo off
concolor -println "setting up ECOTOOLS environment..." green

set ROOT_ECOTOOLS=%CD%
set path=%path%;!ROOT_ECOTOOLS!\Tools

doskey prjroot=cd /d %ROOT_ECOTOOLS%
concolor -println "Macros:" green
doskey /m

concolor -println "done." green
title PROJECT ecotoolsJ (%ROOT_ECOTOOLS%)