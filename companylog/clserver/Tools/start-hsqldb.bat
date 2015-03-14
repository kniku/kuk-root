pushd server
java -cp lib/hsqldb.jar org.hsqldb.Server -database.0 file:data\hsqldb\data -dbname.0 clservdb
popd