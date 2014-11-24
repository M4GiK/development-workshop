<?
	session_start();
	if($_GET["logout"] == "true")
	{
		session_unregister("login");
		header("location:../../index.php");
		exit();
	}
	else
	{
		//session_register("login");
		if(!isset($_SESSION["login"]))
			header("location:login.php");
	}		
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=ISO-8859-1" />
<title>Mi cuenta</title>
</head>

<body>
<h2>Cuenta usuario</h2>
<strong>Nombre:</strong><? echo $_SESSION["login"]["username"];?><br />
  <strong>Mail:</strong><? echo $_SESSION["login"]["mail"]; ?><br />
  <strong>Instituci&oacute;n:</strong><? echo $_SESSION["login"]["institucion"];?><br />
  <strong>Ultimo login:</strong><? echo date("G:i d/m/Y",$_SESSION["login"]["ultimo_login"]);?><br />
  <br />
  <a href="account.php?logout=true">logout</a><br />
  <a href="../../index.php">inicio
  </a>
</body>
</html>
