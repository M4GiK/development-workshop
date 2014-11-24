<?
	session_start();
	if($_GET["logout"] == "true")
	{
		session_unregister("admin");
		header("location:../../index.php");
		exit();
	}
	else
	{
		//session_register("admin");
		if(!isset($_SESSION["admin"]))
			header("location:../login.php");
	}	
	
	
?>
<?	
	include "../../management/configManager.php";
	include "../config.php";
	
	function checkError($params)
	{
		$cm = new configManager();
		$password = $cm->getPassword();
		$errors = array();
		if($params["anterior"] != $password || $params["anterior"] == "")
		{
			$errors[] = "<p align=\"center\"><img src=\"../../frontend/img/icons/exclamation.png\"/>&nbsp;Password anterior incorrecto!<p/>";
		}
		else
		{
			if($params["anterior"] == $params["nuevo"])
				$errors[] = "<p align=\"center\"><img src=\"../../frontend/img/icons/exclamation.png\"/>&nbsp;Password anterior y nuevo iguales !<p/>";
			
			if($params["nuevo"] == "")
			{
				$errors[] = "<p align=\"center\"><img src=\"../../frontend/img/icons/exclamation.png\"/>&nbsp;Tiene que especificar un password<p/>";
			}
			if($params["nuevo"] != $params["repeticion"])
			{
				$errors[] = "<p align=\"center\"><img src=\"../../frontend/img/icons/exclamation.png\"/>&nbsp;Password nuevo y repeticion diferentes!<p/>";
			}			
		}
		
		
		return $errors;
	}
	
	if(isset($_POST["guardar"]))
	{	
		$errors = checkError($_POST);
		
		if (count($errors) == 0)
		{	
			$cm = new configManager();
			$cm->editPassword($_POST["nuevo"]);
			header("location:password_ok.php");
			
		}	
			
	}	
?>	


<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<title>Web GRID</title>
<link href="../../frontend/style-css/style_admin_usuario.css" rel="stylesheet" type="text/css" />
 <!--[if IE 6]>
        <link rel="stylesheet" type="text/css" href="../../frontend/style-css/style_ie6_admin_usuario.css" media="all" charset="utf-8" />
 <![endif]--> 
 <!--[if IE 7]>
        <link rel="stylesheet" type="text/css" href="../../frontend/style-css/style_ie7_admin_usuario.css" media="all" charset="utf-8" />
 <![endif]-->
 
<meta http-equiv="content-language" content="cas" />
</head>

<body>
<?
	include "../menus.php";

?>
<div id="contenido">
	<div id="cabecera">
		<div id="logo"><? include "../header.htm"?></div>
	</div>
	<div id="central">
		<div id="primer_menu">
			<ul class="ul_primermenu">
				<? getFirstMenuConfig();?>
			</ul>
		</div>	
		
	<div id="pestanas"> 
		<ul class="ul_pestana">
			<? getTabsConfig("Password");?>
		</ul>
	</div>
	<div id="parte_central">
	
	
<div id="result" align="center">

<h1>Password</h1>
<div id="error">
<?
	for($i = 0; $i <count($errors);$i++)
	{
		echo $errors[$i];
	}
?>
</div>
<form id="form1" method="post" action="password.php">
<table width="60%" cellspacing="0" cellpadding="0" class="tabladatos">
  <tr>
    <th scope="row" class="th_tabladatos">password anterior</th>
    <td class="td_tabladatos_PAR"><input name="anterior" type="password" id="anterior" /></td>
  </tr>
  <tr>
    <th scope="row" class="th_tabladatos">nuevo password</th>
    <td class="td_tabladatos_PAR"><input name="nuevo" type="password" id="nuevo" /></td>
  </tr>
  <tr>
    <th scope="row" class="th_tabladatos">repite password</th>
    <td class="td_tabladatos_PAR"><input name="repeticion" type="password" id="repeticion" /></td>
  </tr>
  <tr>
   <td colspan="2" class="td_tabladatos_PAR"><input name="guardar" type="submit" id="guardar" value="guardar" /></td>
  </tr>
</table>

</form>
</div>
<br />
<br />

	</div>
</div>
<div id="peu">&nbsp;</div>
</div>

</body>
</html>