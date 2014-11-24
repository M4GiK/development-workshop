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
	include "../config.php";
	include "../../management/configManager.php";
	include "../menus.php";
	
	$cm = new configManager();
	$message = $cm->getMessageAdmin();
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
			<? getTabsConfig("Mensaje admin");?>
		</ul>
	</div>
	<div id="parte_central">
	
	
<div id="result" align="center">

<h1>Mensaje administraci&oacute;n </h1>
<form id="form1" method="post" action="saveMessage.php">
<table width="60%" cellspacing="0" cellpadding="0" class="tabladatos">
  <tr>
    <th scope="row" class="th_tabladatos">Mensaje administraci&oacute;n</th>
    <td class="td_tabladatos_PAR"><textarea name="message" cols="60" rows="10" id="mail"><? echo $message;?></textarea></td>
  </tr>
  <tr>
   <td colspan="2" class="td_tabladatos_PAR"><input name="Submit" type="submit" value="guardar" /></td>
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