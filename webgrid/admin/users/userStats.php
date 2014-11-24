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
        <link rel="stylesheet" type="text/css" href="style-css/style_ie6_admin_usuario.css" media="all" charset="utf-8" />
 <![endif]--> 
 <!--[if IE 7]>
        <link rel="stylesheet" type="text/css" href="style-css/style_ie7_admin_usuario.css" media="all" charset="utf-8" />
 <![endif]-->
 
<meta http-equiv="content-language" content="cas" />
</head>

<body>
<?
	include "../config.php";
	include "../../management/userManager.php";
	include "../../management/incidenceManager.php";
	include "../menus.php";
	
	if(isset($_GET["username"]))
	{
		$userm = new userManager();
		$_SESSION["username"] = $userm->getUser($_GET["username"]);	
	}
	
	if(isset($_POST["session_exec"]))
	{
		$month_fin = $_POST["month_fin"];
		$year_fin = $_POST["year_fin"];
		$month_ini = $_POST["month_ini"];
		$year_ini = $_POST["year_ini"];	
	}
	else
	{
			$actual = getdate(time());
			$month_fin = $actual["mon"];
			$year_fin = $actual["year"];
			
			if($month_fin - 2 <=0)
			{
				$month_ini = 12 + $month_fin -2;
				$year_ini = $year_fin -1;
			}
			else
			{
				$month_ini =  $month_fin - 2;
				$year_ini = $year_fin;
			}
	}
		
	function getSelectMonth($selected)
	{
		for($i = 1; $i <= 12; $i++)
		{
			if ($i != $selected)
				echo "<option value=\"$i\">$i</option>";
			else
				echo "<option value=\"$i\" selected=\"selected\">$i</option>";
		}
	
	}
	
	function getSelectYear($selected)
	{
			
		for($i = 7; $i <= 99; $i++)
		{
			$year = 2000 + $i;
			if ($year != $selected)
			{	
				if($i >= 10)
					echo "<option value=\"$year\">$i</option>";
				else
					echo "<option value=\"$year\">0$i</option>";	
			}	
			else
			{	
				if($i >= 10)
						echo "<option value=\"$year\" selected=\"selected\">$i</option>";
				else
					echo "<option value=\"$year\" selected=\"selected\">0$i</option>";
			}	
		}
	}
	$im = new incidenceManager();
	$num = $im->getNumberWaitingIncidences();
?>
<div id="contenido">
	<div id="cabecera">
		<div id="logo"><? include "../header.htm"?></div>
	</div>
	<div id="central">
		<div id="primer_menu">
			<ul class="ul_primermenu">
				<? getFirstMenuGestion();?>
			</ul>
		</div>	
		
	<div id="pestanas"> 
		<ul class="ul_pestana">
			<? getTabsGestion("Usuarios",$num);?>
		</ul>
	</div>
	<div id="parte_central">
	<div id="menu_interior">
		<ul class="ul_segundomenu">
			<li class="li_segundomenu"><a href="index.php">Volver</a></li>
		</ul>
	</div>	 
<div id="result" align="center">
<h1>Estadisticas usuario <? echo $_SESSION["username"]["username"];?></h1>
<table width="80%" cellpadding="0" cellspacing="0" class="tabladatos">
  <tr>
    <th class="th_tabladatos" scope="col"><strong>Nombre</strong></td>
    <th class="th_tabladatos" scope="col"><strong>Mail</strong></td>
    <th class="th_tabladatos" scope="col"><strong>Direccion</strong></td>
    <th class="th_tabladatos" scope="col"><strong>Ciudad</strong></td>
    <th class="th_tabladatos" scope="col"><strong>Provincia</strong></td>
    <th class="th_tabladatos" scope="col"><strong>Pais</strong></td>
  </tr>
  <tr>
    <td class="td_tabladatos_PAR"><? echo $_SESSION["username"]["username"];?></td>
    <td class="td_tabladatos_PAR"><? echo $_SESSION["username"]["mail"];?></td>
    <td class="td_tabladatos_PAR"><? echo $_SESSION["username"]["direccion"];?></td>
    <td class="td_tabladatos_PAR"><? echo $_SESSION["username"]["ciudad"];?></td>
    <td class="td_tabladatos_PAR"><? echo $_SESSION["username"]["provincia"];?></td>
    <td class="td_tabladatos_PAR"><? echo $_SESSION["username"]["pais"];?></td>
  </tr>
  <tr>
    <th class="th_tabladatos" scope="col"><strong>Fecha alta </strong></td>
    <th class="th_tabladatos" scope="col"><strong>Ultimo_login</strong></td>
    <th class="th_tabladatos" scope="col"><strong>#ejecuciones</strong></td>
    <th class="th_tabladatos" scope="col"><strong>#sesiones</strong></td>
    <th class="th_tabladatos" scope="col"><strong>Ocupacion</strong></td>
    <th class="th_tabladatos" scope="col"><strong>Institucion</strong></td>
  </tr>
  <tr>
    <td class="td_tabladatos_PAR"><? echo date("G:i d/m/Y",$_SESSION["username"]["fecha_alta"]);?></td>
    <td class="td_tabladatos_PAR"><? echo date("G.i d/m/Y",$_SESSION["username"]["ultimo_login"]);?></td>
    <td class="td_tabladatos_PAR"><? echo $_SESSION["username"]["num_ejecuciones"];?></td>
    <td class="td_tabladatos_PAR"><? echo $_SESSION["username"]["num_sesiones"];?></td>
    <td class="td_tabladatos_PAR"><? echo $_SESSION["username"]["ocupacion"];?></td>
    <td class="td_tabladatos_PAR"><? echo $_SESSION["username"]["institucion"];?></td>
  </tr>
  <tr>
    <th class="th_tabladatos" scope="col"><strong>Telefono</strong></td>
    <th class="th_tabladatos" scope="col"><strong>Web</strong></td>
    <th colspan="4" class="th_tabladatos" scope="col"><strong>Interes</strong></td>
  </tr>
  <tr>
    <td class="td_tabladatos_PAR">&nbsp;<? echo $_SESSION["username"]["telefono"];?></td>
    <td class="td_tabladatos_PAR">&nbsp;<? echo $_SESSION["username"]["web"];?></td>
    <td colspan="4" class="td_tabladatos_PAR">&nbsp;<? echo $_SESSION["username"]["interes"];?></td>
  </tr>
</table>
<br />

<br />
<form id="form1" name="form1" method="post" action="userStats.php<? echo"?username=".$_GET["username"]; ?>">
  <select name="month_ini">
  <? getSelectMonth($month_ini);?>
  </select>
  / 20
  <select name="year_ini">
  <? getSelectYear($year_ini);?>
  </select>
entre  
<select name="month_fin">
  <? getSelectMonth($month_fin);?>
  </select>
/ 20 
  <select name="year_fin">
  <? getSelectYear($year_fin);?>
  </select>
  <input type="hidden" name="graph" value="session_exec"/>
  <input type="submit" name="session_exec" value="Ver grafica" />
</form>
  <br />
  <br />
  <img src="buildUserStats.php?graph=session_exec<? echo "&username=".$_GET["username"]."&month_ini=$month_ini&year_ini=$year_ini&month_fin=$month_fin&year_fin=$year_fin"; ?>" width="600"height="400" /><br />
</div>
<br />
<br />

	</div>
</div>
<div id="peu">&nbsp;</div>
</div>
</body>
</html>
