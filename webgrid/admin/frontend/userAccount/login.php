<?
	session_start();
	//session_register("login");
	if(isset($_SESSION["login"]))
		header("location:account.php");
			
	include "../../admin/config.php";
	include "../../management/userManager.php";
	if (isset($_POST["login"]))
	{
		$userm = new userManager();
		
		if($userm->login($_POST["username"],$_POST["password"]))
		{
			
			session_start();
			//session_register("login");
			$_SESSION["login"] = $userm->getUser($_POST["username"]);
			header("location:account.php");
			exit();
		}
		else
			$error = true;
	}
	
?>

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=ISO-8859-1" />
<title>login</title>
</head>

<body>
<table width="500" border="1" align="center" cellspacing="0" bordercolor="#000000">
  <tr>
    <td align="center" bgcolor="#CCCCCC"><h1>LOGIN<br />
    </h1></td>
  <tr>
    <td align="center">
	<? 
		if($error)
			echo "username o password incorrecto!";
	?>
	</td>
  </tr>
  <tr>
  	<td align="center"><form id="form1" method="post" action="login.php">
  <p><br />
    username 
      <input name="username" type="text" id="username" />
    <br />
    <br />
    password 
    <input name="password" type="password" id="password" />
    <br />
    <br />
    <input type="submit" name="login" value="login" />
    <br />
  </p>
</form></td>
  </tr>
  <tr>
    <td align="center"><a href="formRegister.php">Registrate</a></td>
  </tr>
</table>
<br />
<a href="../../index.php">volver
</a>
</body>
</html>
