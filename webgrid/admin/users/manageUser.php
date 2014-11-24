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
	
	$action = $_GET["action"];
	$username = $_GET["username"];
	
	$userm = new userManager();
	switch($action)
	{
		case "edit":
					//echo "section:$section subsection:$subsection<br/>";
					$info = $userm->getUser($username);
					//print_r($info);
					
					break;
		case "delete":
						break;
		case "newSection":
							break;
		
		case "newSubsection":
								break;												
	}
	
	function getCountries($selected)
	{
		$userm = new userManager();
		$countries = $userm->getCountries();
		
		for($i = 0; $i< count($countries); $i++)
		{
			$country = $countries[$i]["name"];
			if($country == $selected)
				echo "<option value=\"$country\" selected=\"selected\">$country</option>";
			else
				echo "<option value=\"$country\">$country</option>";		
		}
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
	
	$im = new incidenceManager();
	$num = $im->getNumberWaitingIncidences();
?>
<<div id="contenido">
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

<h1 align="center"> Editar usuarios<br />
</h1>
<form action="saveUser.php" method="post" name="form1" id="form1">
  <table width="80%" cellpadding="0" cellspacing="0" class="tabladatos">
    <tr>
      <th class="th_tabladatos" scope="row"><strong>username</strong></td>
      <td class="td_tabladatos_PAR"><input name="username" type="text" id="username" value="<? echo $info["username"];?>" /> 
      <br /></td>
    </tr>
    <tr>
      <th class="th_tabladatos" scope="row"><strong>mail</strong></td>
      <td class="td_tabladatos_PAR"><input name="mail" type="text" id="mail" value="<? echo $info["mail"]; ?>" />
      <br /></td>
    </tr>
    <tr>
      <th class="th_tabladatos" scope="row"><strong>password</strong></td>
      <td class="td_tabladatos_PAR"><input name="password" type="text" id="password" value="<? echo $info["password"];?>"/>
      <br /></td>
    </tr>
	 <tr>
      <th class="th_tabladatos" scope="row"><strong>pais</strong></td>
    <td class="td_tabladatos_PAR"><select name="pais" id="pais" value="<? echo $info["pais"];?>">
								<? getCountries($info["pais"])?>
								</select></td></tr>
	 <tr>
       <th class="th_tabladatos" scope="row"><strong>provincia</strong></td>
	   <td class="td_tabladatos_PAR"><input name="provincia" type="text" id="provincia" value="<? echo $info["provincia"];?>" />
           <br /></td>
    </tr>
	 <tr>
       <th class="th_tabladatos" scope="row"><strong>ciudad</strong></td>
	   <td class="td_tabladatos_PAR"><input name="ciudad" type="text" id="ciudad" value="<? echo $info["ciudad"]; ?>" />
           <br /></td>
    </tr>
	 <tr>
       <th class="th_tabladatos" scope="row"><strong>direccion</strong></td>
	   <td class="td_tabladatos_PAR"><input name="direccion" type="text" id="direccion" value="<? echo $info["direccion"];?>"/>
           <br /></td>
    </tr>
	 <tr>
       <th class="th_tabladatos" scope="row"><strong>telefono</strong></td>
	   <td class="td_tabladatos_PAR"><input name="telefono" type="text" id="telefono" value="<? echo $info["telefono"];?>"/>
	       <br />       </td>
    </tr>
	 <tr>
       <th class="th_tabladatos" scope="row"><strong>web</strong></td>
	   <td class="td_tabladatos_PAR"><input name="web" type="text" id="web" value="<? echo $info["web"];?>" />
           <br /></td>
    </tr>
	 <tr>
       <th class="th_tabladatos" scope="row"><strong>ocupacion</strong></td>
	   <td class="td_tabladatos_PAR"><select name="ocupacion" id="ocupacion" value="<? echo $info["ocupacion"]; ?>" >
	   								<? getOptions("Cualquiera",array("","Engineer","Researcher","Professor","Ms Student","Ph.D. Student","Undergraduate Student","Other"),array("","Engineer","Researcher","Professor","Ms Student","Ph.D. Student","Undergraduate Student","Other"),$info["ocupacion"]);?>
									</select>
           <br /></td>
    </tr>
	 <tr>
       <th class="th_tabladatos" scope="row"><strong>interes</strong></td>
	   <td class="td_tabladatos_PAR"><textarea name="interes" rows="10"><? echo $info["interes"];?></textarea>
       <br /></td></tr>
	 <tr>
	   <td colspan="2" align="center" valign="top" bordercolor="#333333"><input type="hidden" name="user"  value="<? echo "$username";?>"/></td>
    </tr>
    <tr>
      <th class="th_tabladatos" scope="row"><strong>institucion</strong></td>
      <td class="td_tabladatos_PAR"><input name="institucion" type="text" id="institucion" value="<? echo $info["institucion"];?>" />
          <br /></td>
    </tr>
    <tr>
      <th class="th_tabladatos" scope="row"><strong>pais institucion </strong></td>
      <td class="td_tabladatos_PAR"><input name="pais_ins" type="text" id="pais_ins" value="<? echo $info["pais_ins"]; ?>" />
          <br /></td>
    </tr>
    <tr>
      <th class="th_tabladatos" scope="row"><strong>provincia ins </strong></td>
      <td class="td_tabladatos_PAR"><input name="provincia_ins" type="text" id="provincia_ins" value="<? echo $info["provincia_ins"];?>"/>
          <br /></td>
    </tr>
    <tr>
      <th class="th_tabladatos" scope="row"><strong>direccion ins </strong></td>
      <td class="td_tabladatos_PAR"><input name="direccion_ins" type="text" id="direccion_ins" value="<? echo $info["direccion_ins"];?>"/>
          <br /></td>
    </tr>
    <tr>
      <th class="th_tabladatos" scope="row"><strong>telefono ins </strong></td>
      <td class="td_tabladatos_PAR"><input name="telefono_ins" type="text" id="telefono_ins" value="<? echo $info["telefono_ins"];?>" />
          <br /></td>
    </tr>
    <tr>
      <th class="th_tabladatos" scope="row"><strong>web ins </strong></td>
      <td class="td_tabladatos_PAR"><input name="web_ins" type="text" id="web_ins" value="<? echo $info["web_ins"]; ?>" />
          <br /></td>
    </tr>
	<tr>
		<td colspan="2" class="td_tabladatos_PAR"><input type="submit" name="Submit" value="guardar" /></td>
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