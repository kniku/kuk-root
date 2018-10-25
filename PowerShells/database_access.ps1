$con=[System.Data.SqlClient.SqlConnection]::new('Server=ax-nb-65\sqlserver;Database=DEV;User Id=sa;Password=sa;Connection Timeout=3;');
$con.Open();
$sql=[System.Data.SqlClient.SqlCommand]::new('select count(*) from sys.tables',$con);
$sql.ExecuteScalar();

$sql.CommandText='select * from sys.tables';
$SqlAdapter = [System.Data.SqlClient.SqlDataAdapter]::new($sql);
$DataSet = [System.Data.DataSet]::new();
$SqlAdapter.Fill($DataSet);
$DataSet.Tables[0]

$con.Close();

