<?
	session_start();
	//session_register("login");
	if(isset($_SESSION["login"]))
		header("location:executions.php");
			
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
			header("location:executions.php");
			exit();
		}
		else
			$error = true;
	}
	
?>

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<title>Web GRID</title>
<link href="../style-css/style_admin.css" rel="stylesheet" type="text/css" />
 <!--[if IE 6]>
        <link rel="stylesheet" type="text/css" href="style-css/style_ie6_admin.css" media="all" charset="utf-8" />
 <![endif]--> 
 <!--[if IE 7]>
        <link rel="stylesheet" type="text/css" href="style-css/style_ie7_admin.css" media="all" charset="utf-8" />
 <![endif]-->
 
<meta http-equiv="content-language" content="en" />
</head>


<body>
<div id="contenido">
	<div id="cabecera" align="center"><div id="logo"><img src="../img/micuenta_eng.gif" alt="mi cuenta" /></div></div>
	<div id="central">
		
		<div id="error">
		<? 
			if($error)
				echo "username o password incorrecto!</br>";
		?> 
		</div>
		
		<div id="login">
			<form id="form1" method="post" action="login.php">
			 <p class="user"> user&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; 
				 <input name="username" type="text" id="username" /></p>
	  
			<p class="user"> password &nbsp;
				<input name="password" type="password" id="password" /></p> 
			
			<p class="user">
				<input type="submit" name="login" value="login" />
		  </form>
			</p>
			<div id="enlaces">
			<ul>
				<li><a href="formRegister.php">Register now</a></li>
				<li><a href="../../index.php">back</a></li>
			</ul>
		</div>
		</div>
		
	</div>
	<div id="peu">&nbsp;</div>
</div>
</body>
</html>
