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
	
	if (isset($_POST["filtrar"]))
	{
		
		$filter["pais"] = $_POST["pais"];
		$filter["ocupacion"] = $_POST["ocupacion"];
		
		//fecha de alta
		if($_POST["mes_ini_fecha_alta"]!=0 && $_POST["dia_ini_fecha_alta"]!=0 && $_POST["year_ini_fecha_alta"]!=0 && $_POST["mes_fin_fecha_alta"]!=0 && $_POST["dia_fin_fecha_alta"]!=0 && $_POST["year_fin_fecha_alta"]!=0)
		{
			$filter["fecha_alta"]["ini"] = mktime(0,0,0,$_POST["mes_ini_fecha_alta"],$_POST["dia_ini_fecha_alta"],$_POST["year_ini_fecha_alta"]);
			$filter["fecha_alta"]["fin"] = mktime(0,0,0,$_POST["mes_fin_fecha_alta"],$_POST["dia_fin_fecha_alta"],$_POST["year_fin_fecha_alta"]);	
		}
		//fecha de login
		if($_POST["mes_ini_ultimo_login"]!=0 && $_POST["dia_ini_ultimo_login"]!=0 && $_POST["year_ini_ultimo_login"]!=0 && $_POST["mes_fin_ultimo_login"]!=0 && $_POST["dia_fin_ultimo_login"]!=0 && $_POST["year_fin_ultimo_login"]!=0)
		{
		
			$filter["ultimo_login"]["ini"] = mktime(0,0,0,$_POST["mes_ini_ultimo_login"],$_POST["dia_ini_ultimo_login"],$_POST["year_ini_ultimo_login"]);
			$filter["ultimo_login"]["fin"] = mktime(0,0,0,$_POST["mes_fin_ultimo_login"],$_POST["dia_fin_ultimo_login"],$_POST["year_fin_ultimo_login"]);	
		}
		
		$_SESSION["filter"]["filter"] = $filter;
		$_SESSION["filter"]["post"] = $_POST;
			
	}

	
		
	function addRowUser($user,$class)
	{
		$username = $user["username"];
		$fecha = date("G:i d/m/Y",$user["fecha_alta"]);
		
		if($user["ultimo_login"] == 0)
			$ultimo_login = "nunca";
		else	
			$ultimo_login = date("G:i d/m/Y",$user["ultimo_login"]);
		
		$ocupacion = $user["ocupacion"];
		$pais = $user["pais"];
		$num_ejecuciones = $user["num_ejecuciones"];
		$num_sesiones = $user["num_sesiones"];
		
		echo "
				<tr>
    				<td class=\"$class\">$username</td>
					<td class=\"$class\">$fecha</td>
					<td class=\"$class\">$ultimo_login</td>
					<td class=\"$class\">$ocupacion</td>
					<td class=\"$class\">$pais</td>
					<td class=\"$class\">$num_ejecuciones</td>
					<td class=\"$class\">$num_sesiones</td>
    				<td class=\"$class\"><a href=\"manageUser.php?action=edit&username=$username\">modificar</a> <a href=\"removeUser.php?username=$username\">eliminar </a> 
					<a href=\"userStats.php?username=$username\">estadisticas </a>
					</td>
				</tr>";
	}
	
	function getUsersInfo($page,$filter, $order)
	{
		$userm = new userManager();
		$users = $userm->getUsers($filter,$order);
		
		if($page !="")
		{
			$i = ($page -1) * 8;
			$fin = $page * 8;
			
		}
		else
		{
			$i = 0;
			$fin = 8;
		}
		
		$pages = ceil(count($users)/8);
		
		while($i<$fin && $i < count($users))
		{
			if($i % 2 != 0)
				$class= "td_tabladatos_IMPAR"; 
			else	
				$class= "td_tabladatos_PAR"; 
			
			addRowUser($users[$i],$class);
			$i++;
		}
		
		return $pages;
	}

	function getCountries($selected)
	{
		$userm = new userManager();
		$countries = $userm->getCountries();
		
		if($selected == "" || $selected == "0")
			echo "<option value=\"0\" selected =\"selected\">Cualquiera</option>";	
		else
			echo "<option value=\"0\" >Cualquiera</option>";	
		
		for($i = 0; $i< count($countries); $i++)
		{
			$country = $countries[$i]["name"];
			if($country == $selected)
				echo "<option value=\"$country\" selected=\"selected\">$country</option>";
			else
				echo "<option value=\"$country\">$country</option>";		
		}
	}
	

	function getHeader($default,$values)
	{
		echo "<tr>";
		if(isset($_GET["order"]) && isset($_GET["way"]))
		{
		 	$order = $_GET["order"];
		 	$way = $_GET["way"];
			
			if($way == "ASC")
			{	
				$img = "../../frontend/img/arrow_u.gif";				
				$waylink = "DESC";
			}	
			else
			{	
				$img = "../../frontend/img/arrow_rx.gif";
				$waylink = "ASC";
			}			
		}
		else
		{	
			$order = $default;
			$way = "ASC";
			$waylink = "DESC";			
			$img = "../../frontend/img/arrow_u.gif";
		}
		
		for($i = 0; $i < count($values); $i++)
		{
			if($order == $values[$i])
			{
				echo "<th class=\"th_tabladatos\" scope=\"col\"><strong><a href=\"index.php?order=$order&way=$waylink\">[$order] </a>		 <img src=\"$img\" alt=\"\" width=\"7\" height=\"7\" /></strong></th>";
			}
			else
			{
				echo "<th class=\"th_tabladatos\" scope=\"col\"><strong><a href=\"index.php?order=$values[$i]&way=ASC\">$values[$i]</a></strong></th>";			}	
		
		}
		
		echo "<th class=\"th_tabladatos\" scope=\"col\"><strong>acciones</strong></th>";			
		
		
		
		$info["order"] = $order;
		$info["way"] = $way;
		
		return $info;
		
	}		
	
	function getOptions($ini, $keys, $values, $selected)
	{
	
		if($selected == "0" || $selected == "")
			echo "<option value=\"0\" selected=\"selected\">$ini</option>";
		else
			echo "<option value=\"0\" >$ini</option>";
			
		for($i = 1; $i < count($keys); $i++)
		{
				if($values!= "")
				{
					if ($values[$i] != $selected)
						echo "<option value=\"$values[$i]\">$values[$i]</option>";
					else
						echo "<option value=\"$values[$i]\" selected=\"selected\">$values[$i]</option>";
				}
				else
				{
					if ($i != $selected)
						echo "<option value=\"$i\">$i</option>";
					else
						echo "<option value=\"$i\" selected=\"selected\">$i</option>";	
				}		
				
		}
	}
	
	function getOptionsYear($selected)
	{
		if($selected == "0" || $selected == "")
			echo "<option value=\"0\" selected=\"selected\">-</option>";
		else
			echo "<option value=\"0\" >-</option>";
			
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
	
	
	function getPagination($pages,$actual,$order)
	{
		$order_by = $order["order"];
		$way = $order["way"];
		if($actual == "")
			$actual = 1;
		
		for($i = 1; $i <=$pages; $i++)
		{
			if($actual== $i)
				echo "<strong>$i</strong>&nbsp;";
			else
				echo "<a href=\"index.php?page=$i&order=$order_by&way=$way\">$i</a>&nbsp;";	
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
			<li class="li_segundomenu"><span class="segundomenu_selec">Ver usuarios</span></li>
			<li class="li_segundomenu"><a href="stats.php">Ver estad&iacute;sticas</a></li>
		</ul>
	</div>	 
<div id="result" align="center">

<h1 align="center"> Usuarios<br />
</h1>
<form id="form1" method="post" action="index.php<?
if(isset($_GET["page"]))
	echo "?page=".$_GET["page"];
else
	echo "?page=1";

if(isset($_GET["order"]) && isset($_GET["way"]))
	echo "&order=".$_GET["order"]."&way=".$_GET["way"];				
?>">
  <table width="750" cellspacing="0">
    <tr>
      <td width="171" bgcolor="#EFEFEF">filtrar por pais: </td>
      <td width="569">
	  	<select name="pais" id="pais">
		<?
			getCountries($_SESSION["filter"]["post"]["pais"]);	
		?>
	  	</select>				
	  </td>
    </tr>
    <tr>
      <td>filtrar por fecha de alta: </td>
      <td valign="middle"><select name="dia_ini_fecha_alta" id="dia_ini_fecha_alta">
      <? 
	  	
		getOptions("-",array_fill(0,32,0),"",$_SESSION["filter"]["post"]["dia_ini_fecha_alta"]);
	  ?>
      </select>
      /
        <select name="mes_ini_fecha_alta" id="mes_ini_fecha_alta">
        <? 
		
			getOptions("-",array_fill(0,13,0),"",$_SESSION["filter"]["post"]["mes_ini_fecha_alta"]);
		?>
		</select>
        /
        
		20
		<select name="year_ini_fecha_alta" id="year_ini_fecha_alta" >
			<? 
			
				getOptionsYear($_SESSION["filter"]["post"]["year_ini_fecha_alta"]);	
			?>
        
		</select> 
		entre 
		<select name="dia_fin_fecha_alta" id="dia_fin_fecha_alta">
        <? 
		
			getOptions("-",array_fill(0,32,0),"",$_SESSION["filter"]["post"]["dia_fin_fecha_alta"]);
		?>
		</select>
        /
        <select name="mes_fin_fecha_alta" id="mes_fin_fecha_alta">
        <? 
		
			getOptions("-",array_fill(0,13,0),"",$_SESSION["filter"]["post"]["mes_fin_fecha_alta"]);
		?>
		</select>
        /
      20
        <select name="year_fin_fecha_alta" id="year_fin_fecha_alta" >
	  	<? 
		
			getOptionsYear($_SESSION["filter"]["post"]["year_fin_fecha_alta"]);		
		?>
    
	  </select></td>
    </tr>
    <tr>
      <td bgcolor="#EFEFEF">filtrar por ultimo login </td>
      <td><select name="dia_ini_ultimo_login" id="dia_ini_ultimo_login">
          <? 
		  	getOptions("-",array_fill(0,32,0),"",$_SESSION["filter"]["post"]["dia_ini_ultimo_login"]);
			
		?>
		  </select>
      /
          <select name="mes_ini_ultimo_login" id="mes_ini_ultimo_login">
                   <?
				   		getOptions("-",array_fill(0,13,0),"",$_SESSION["filter"]["post"]["mes_ini_ultimo_login"]);
				   ?>
	    </select>
          / 20
          <select name="year_ini_ultimo_login" id="year_ini_ultimo_login" >
		  <? 
	
		  	getOptionsYear($_SESSION["filter"]["post"]["year_ini_ultimo_login"]);		
		  ?>
		  </select>
          entre
          <select name="dia_fin_ultimo_login" id="dia_fin_ultimo_login">
                   <? 
				   		getOptions("-",array_fill(0,32,0),"",$_SESSION["filter"]["post"]["dia_fin_ultimo_login"]);
				   ?>
	    </select>
          /
          <select name="mes_fin_ultimo_login" id="mes_fin_ultimo_login">
                 <? 
				 	getOptions("-",array_fill(0,13,0),"",$_SESSION["filter"]["post"]["mes_fin_ultimo_login"]);
				 ?>
		</select>
          / 20
          <select name="year_fin_ultimo_login" id="year_fin_ultimo_login" >
		 <? 
		 getOptionsYear($_SESSION["filter"]["post"]["year_fin_ultimo_login"]);		
		 ?>
		  </select>
	  </td>
    </tr>
    <tr>
      <td>filtrar por ocupacion: </td>
      <td><select name="ocupacion" id="ocupacion" >
        <? 
			getOptions("Cualquiera",array("","Engineer","Researcher","Professor","Ms Student","Ph.D. Student","Undergraduate Student","Other"),array("","Engineer","Researcher","Professor","Ms Student","Ph.D. Student","Undergraduate Student","Other"),$_SESSION["filter"]["post"]["ocupacion"]);	
		?>
      </select>      </td>
    </tr>
    <tr>
      <td colspan="2">&nbsp;</td>
    </tr>
	<tr>
      <td colspan="2"><input name="filtrar" type="submit" id="filtrar" value="aplicar filtro" /></td>
    </tr>
  </table>
  
</form>
<br />
<br />
<table width="700" cellpadding="0" cellspacing="0" class="tabladatos">
  
 <?
	
	$order = getHeader("username",array("username","fecha_alta","ultimo_login","ocupacion","pais","num_ejecuciones","num_sesiones"));	
	$pages = getUsersInfo($_GET["page"],$_SESSION["filter"]["filter"], $order);
 ?>
  <tr>
    <td colspan="8" class="td_paginacion"><? getPagination($pages,$_GET["page"],$order); ?></td>
  </tr>
  <tr class="tr_tabladatosfin">
      <td></td>
      <td></td>
      <td></td>
      <td></td>
      <td></td>
      <td></td>
      <td></td>
	  <td></td>
    </tr>
</table>
</div>
<br />
<br />

	</div>
</div>
<div id="peu">&nbsp;</div>
</div>
</body>
</html>
