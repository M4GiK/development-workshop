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

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<title>Web GRID</title>
<link href="../style-css/style_admin_usuario.css" rel="stylesheet" type="text/css" />
<!--[if IE 6]>
        <link rel="stylesheet" type="text/css" href="../style-css/style_ie6_admin_usuario.css" media="all" charset="utf-8" />
 <![endif]--> 
 <!--[if IE 7]>
        <link rel="stylesheet" type="text/css" href="../style-css/style_ie7_admin_usuario.css" media="all" charset="utf-8" />
 <![endif]-->
 
<meta http-equiv="content-language" content="cas" />
</head>

<body>
<div id="contenido">
	<div id="cabecera">
		<div id="logo"><img src="../img/cuenta_usuario.gif" alt="Cuenta Usuario" /></div>
	</div>
	<div id="central">
		<div id="primer_menu">
			<ul class="ul_primermenu">
				<li class="li_primermenu"><a href="../../index.php?section=1" class="link_primermenu">Inicio</a></li>
				<li class="li_primermenu"><a href="incidences.php?logout=true" class="link_primermenu">Logout</a></li>
			</ul>
		</div>	
		<div id="destacado_gris">
			<ul>
				<li class="li_datos"><strong><? echo $_SESSION["login"]["username"];?></strong></li>
				<li class="li_datos"><strong> Mail:</strong> <? echo $_SESSION["login"]["mail"]; ?></li>
				<li class="li_datos"><strong> Institution:</strong><? echo $_SESSION["login"]["institucion"];?></li>
				<li class="li_datos"><strong> Last login:</strong><? echo date("G:i d/m/Y",$_SESSION["login"]["ultimo_login"]);?></li>
			</ul>
		</div>
	
	<div id="pestanas"> 
		<ul class="ul_pestana">
			<li class="pestanas"><a href="executions.php">Executions</a></li>
			<li class="pestana_selec"><strong>Incidences</strong></li>
			<li class="pestanas"><a href="preferences.php">Settings</a></strong></li>
		</ul>
	</div>
	<div id="parte_central">
	<div id="menu_interior">
		<ul class="ul_segundomenu">
			<li class="li_segundomenu"><a href="incidences.php">My incidences</a></li>
			<li class="li_segundomenu"><span class="segundomenu_selec">New incidence</span></li>
		</ul>
	</div>	 
<div id="result" align="center" >
<h1>New incidence</h1>
<? 
	echo "<p align=\"center\"><img src=\"../../frontend/img/icons/accept.png\"/>&nbsp;The incidence was successfully sent</p>";
	echo "<p align=\"center\"><img src=\"../../frontend/img/icono_amarill_square.gif\"/><a href=\"preferences.php\">&nbsp;back</a></p>";
?>
</div>
<br />
<br />

	</div>
</div>
<div id="peu">&nbsp;</div>
</div>
</body>
</html>
