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
<?
	$dic_params = array("(--nb_evolution_steps" => "Number of evolution steps", "--max_time_to_spend" => "Max time to spend");
	
	include "../../management/configManager.php";
	include "../../management/userManager.php";
	include "../../admin/config.php";
	include "genExecution.php";
	require "../../lib/phpmailer/class.phpmailer.php";
	include "../../lib/Mail.php";
	
	function checkPermision()
	{
		$user = $_SESSION["login"]["username"];

		$userm = new userManager();
		$cm = new configManager();
		
		$max = $cm->getMaxExecutions();
		
		$actual = getdate(time());
		$ini = mktime(0,0,0,$actual["mon"],$actual["mday"],$actual["year"]);
		$fin = mktime(23,59,59,$actual["mon"],$actual["mday"],$actual["year"]);
		
		$executions = $userm->getNumberExecutions($ini,$fin,$user); 
		//echo "max:$max - executions:$executions";
		if($max < $executions)
			header("location:message_params.php?message=max");
		
		//echo $cm->getMaxExecutions()." - ". $userm->getNumberExecutions($ini,$fin,$user);
	}
	
	
	function checkIntegers(&$params, $default,&$errors)
	{
		global $dic_params;
	
		foreach($params as $key => $value)
		{
			switch($key)
			{						
				case "--max_time_to_spend":			
				case "--nb_evolution_steps":					
								if(!is_numeric($value))
									$errors[] = "<p align=\"left\"><img src=\"../../frontend/img/icons/exclamation.png\"/>&nbsp; $dic_params[$key]  	value should be numeric <p/>";				
								else
								{
									$params[$key] = intval($value);
									if($default["max_".$key] < $value || $default["min_".$key] > $value)
										$errors[] = "<p align=\"left\"><img src=\"../../frontend/img/icons/exclamation.png\"/>&nbsp; $dic_params[$key]  	must be in the interval: [".$default["min_".$key]." - ".$default["max_".$key]."]<p/>";
								
								}								
								break;
			}
			if($params["--leader_task"] < 0 || $params["--leader_task"] > 1)
			{
				$errors[] = "<p align=\"left\"><img src=\"../../frontend/img/icons/exclamation.png\"/>&nbsp; Leader task schould be between 0 and 1 value <p/>";
			}
		}		
	}
	
	function checkDistributionValues(&$params,&$errors)
	{
		
		global $dic_params;
		foreach($params as $key => $value)
		{
			
			switch($value)
			{	
				case "c":
							
							if(!is_numeric($params[$key."_constant"]) && $params[$key] != "0")
								$errors[] = "<p align=\"left\"><img src=\"../../frontend/img/icons/exclamation.png\"/>&nbsp; The  $dic_params[$key] constant value should be numeric<p/>";			 
							
							break;
				case "u":
							if(!is_numeric($params[$key."_low"]))
								$errors[] = "<p align=\"left\"><img src=\"../../frontend/img/icons/exclamation.png\"/>&nbsp;  The  $dic_params[$key] low value should be numeric <p/>";
							
							if(!is_numeric($params[$key."_high"]))
								$errors[] = "<p align=\"left\"><img src=\"../../frontend/img/icons/exclamation.png\"/>&nbsp;  The  $dic_params[$key] high value should be numeric <p/>";
											 
							break;
				case "e":
								if(!is_numeric($params[$key."_mean"]))
									$errors[] = "<p align=\"left\"><img src=\"../../frontend/img/icons/exclamation.png\"/>&nbsp;  The  $dic_params[$key] mean value should be numeric <p/>";
							break;
				case "zipf":
							if(!is_numeric($params[$key."_x1"]))
									$errors[] = "<p align=\"left\"><img src=\"../../frontend/img/icons/exclamation.png\"/>&nbsp;  The  $dic_params[$key] x1 value should be numeric <p/>";
							
							if(!is_numeric($params[$key."_x2"]))
									$errors[] = "<p align=\"left\"><img src=\"../../frontend/img/icons/exclamation.png\"/>&nbsp;  The  $dic_params[$key] x2 value should be numeric <p/>";
									
							if(!is_numeric($params[$key."_x3"]))
									$errors[] = "<p align=\"left\"><img src=\"../../frontend/img/icons/exclamation.png\"/>&nbsp;  The  $dic_params[$key] x3 value should be numeric <p/>";				
							break;
				case "n":
							if(!is_numeric($params[$key."_mean"]))
									$errors[] = "<p align=\"left\"><img src=\"../../frontend/img/icons/exclamation.png\"/>&nbsp;  The  $dic_params[$key] mean value should be numeric <p/>";				
							$str .= "n(".$params[$param."_mean"].",".$params[$param."_sd"].")";
							break;
							
							if(!is_numeric($params[$key."_sd"]))
									$errors[] = "<p align=\"left\"><img src=\"../../frontend/img/icons/exclamation.png\"/>&nbsp;  The  $dic_params[$key] sd value should be numeric <p/>";				
				
				case "triangle":
							if(!is_numeric($params[$key."_a"]))
									$errors[] = "<p align=\"left\"><img src=\"../../frontend/img/icons/exclamation.png\"/>&nbsp;  The  $dic_params[$key] a value should be numeric <p/>";
							
							if(!is_numeric($params[$key."_b"]))
									$errors[] = "<p align=\"left\"><img src=\"../../frontend/img/icons/exclamation.png\"/>&nbsp;  The  $dic_params[$key] b value should be numerico <p/>";
									
							if(!is_numeric($params[$key."_c"]))
									$errors[] = "<p align=\"left\"><img src=\"../../frontend/img/icons/exclamation.png\"/>&nbsp; The  $dic_params[$key] c value should be numeric <p/>";		
							$params[$key."_a"] = intval($params[$key."_a"]);						
							$params[$key."_b"] = intval($params[$key."_b"]);
							$params[$key."_c"] = intval($params[$key."_c"]);														
							break;
			}
		}	
	}
	
	function check(&$params,$default)
	{
		
		$errors = array();
		checkIntegers($params,$default,$errors);
		//checkDistributionValues($params,$errors);
		return $errors;
	}
	
	$cm = new configManager();
	$params = $cm->getDefaultParamsWithParametro("default_params_SG");
	
	if(isset($_COOKIE["params"]))
	{
		$old_params = $_COOKIE["params"];
	}
	else
	{
		header("location:params.php");
	}
	
	checkPermision($cm,$user);
	
	if(isset($_POST["Submit"]))
	{
			$errors = check($_POST,$params);
			if(count($errors) == 0)
			{
				if(isset($_COOKIE["params"]))
				{
					$old_params = $_COOKIE["params"];
				}
				ob_start();
				//foreach($_POST as $param => $value)
				{
				//	echo "<br/>".$param."  ".$value."<br/>";
				}
				if($_POST["group1"] == "GA")
				{
					foreach($_POST as $param => $value)
					{
						//echo $param." ".$value."<br/>";
						switch($param)
						{
							case "--number_players":
								$value = $value - 1;
								$strparams .= "--sg_number_followers ".$value ." ";
								break;
							case "--leader_task":
								$strparams .= "--sg_leader_factor ".$value." ";
								break;
							case "--game_scenario":
								if($value == "symmetric")
								{
									$strparams .= "--sg_leader_factor 0 ";//". 1/$_POST["--number_players"]." ";
									$strparams .= "--sg_enable_game 0 ";
								}
								else
									$strparams .= "--sg_enable_game 1 ";	
								break;
							case "--execution_cost":
								echo $param." ".$value;
								if($value == "enable")
									$strparams .= "--sg_enable_qle 1 ";
								else
									$strparams .= "--sg_enable_qle 0 ";
							break;
							case "--resource_utilization_cost":
								if($value == "enable")
									$strparams .= "--sg_enable_qlu 1 ";
								else
									$strparams .= "--sg_enable_qlu 0 ";
								break;
							case "--security_cost":
								if($value == "enable")
									$strparams .= "--sg_security 1 ";
								else
									$strparams .= "--sg_security 0";
								break;
							case "--task_abortion_cost":
								if($value == "enable")
									$strparams .= "--sg_enable_qlab 1 ";
								else
									$strparams .= "--sg_enable_qlab 0";
								break;
									
						}
					}
					$strparams .= "--sg_security_extra 1 --sg_calc_penalty 0 --sg_abort_extra 1 --sg_min_fun_choice 0 ";
					$strparams .= "--sg_independent_GA 0 ";
					//echo $strparams;// = getParams($_POST);
					setcookie('SG_params', $strparams, time() + 3600);
					header("location:GeneticAlgorithmParams.php");
				}
				else
				{
					//generateExecution($_SESSION["login"]["username"],$_POST, $old_params, 0);
				}
				/*$avisos = $cm->getAvisos();
				if($avisos["ejecucion"] == "S") 
				{
					$to = $cm->getMail();
					$message = "El usuario:<strong> ".$_SESSION["login"]["username"]."</strong> ha realizado una ejecucion<br><br> Administracion WEBGRID"; 
					$alt_message = "El usuario:".$_SESSION["login"]["username"]."</strong> ha realizado una nueva ejecucion\n\n Administracion WEBGRID"; 
					$mail = new Mail("WEB GRID@UPC",$to,"Aviso: nueva ejecucion",$message,$alt_message, "../../lib/phpmailer/");
					$mail->send();
				}*/
				ob_end_flush();
					
			}	
			
				
			
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
<script language="JavaScript" type="text/javascript" >
function loadProperties(name,selected)
{
	div = document.getElementById(name);
	var html;
	var name1,name2,name3;
	switch(selected.value)
	{
		case "c":
					name1 = selected.name +  '_constant';
					html = 'constant: <input name=\"'+ name1 +'\" type=\"text\" size=\"4\" />';
					break;
		case "u":
					name1 = selected.name +  '_low';
					name2 = selected.name +  '_high';
					html = 'low: <input name=\"'+ name1 + '\" type=\"text\" size=\"4\" /> high: <input name=\"' + name2 + '\" type=\"text\" size=\"4\" />';
					break;
		case "n":
					name1 = selected.name +  '_mean';
					name2 = selected.name +  '_sd';
					html = 'mean: <input name=\"' + name1 +'\" type=\"text\" size=\"4\" /> sd: <input name=\"' + name2 + '\" type=\"text\" size=\"4\" />';
					break;
		case "e":
					name1 = selected.name + '_mean';
					html = 'mean: <input name=\"'+name1+'\" type=\"text\" size=\"4\" />';
					break;
					
		case "triangle":
						name1 = selected.name +  '_a';
						name2 = selected.name +  '_b';
						name3 = selected.name +  '_c';
						html = 'a: <input name=\"' + name1 + '\" type=\"text\" size=\"4\" /> b: <input name=\"'+ name2 +'\" type=\"text\" size=\"4\" /> c: <input name=\"' + name3 +'\" type=\"text\" size=\"4\" />';
							break;
		case "zipf":	
						name1 = selected.name +  '_x1';
						name2 = selected.name +  '_x2';
						name3 = selected.name +  '_p';
						html = 'x1: <input name=\"'+ name1 +'\" type=\"text\" size=\"4\" /> x2: <input name=\"'+ name2 +'\" type=\"text\" size=\"4\" /> p: <input name=\"'+ name3 +'\" type=\"text\" size=\"4\" />';
						break;
		case "stackelberg":
					html = '<input name=\"--leader_task\" type=\"text\" size=\"4\" value=\"0.5\"/>';
					break;
		case "symmetric":
					html = 'Enable only in stackelberg mode';
					break;
	}	
	//alert(html);
	div.innerHTML = html;		
}	
</script>

</head>

<body>
<?
	
	function getOptions($keys, $values, $selected)
	{
			
		for($i = 0; $i < count($keys); $i++)
		{
				if($values!= "")
				{
					if ($values[$i] != $selected)
						echo "<option value=\"$values[$i]\">$keys[$i]</option>";
					else
						echo "<option value=\"$values[$i]\" selected=\"selected\">$keys[$i]</option>";
				}
				
				
		}
	}
	function getProperties($key, $value, $params)
	{
		switch($value)
			{
				case "c": 
							echo "constant: <input name=\"".$key."_constant\" type=\"text\" size=\"4\" value=\"".$params[$key."_constant"]."\"/>";
							break;
				case "u":	
							echo "low: <input name=\"".$key."_low\" type=\"text\" size=\"4\"  value\"".$params[$key."_low"]."\"/> 
							high: <input name=\"".$key."_high\" type=\"text\" size=\"4\" value\"".$params[$key."_high"]."\"/>";
							break;
								
							
				case "e":
							echo "mean: <input name=\"".$key."_mean\" type=\"text\" size=\"4\" value=\"".$params[$key."_mean"]."\"/>";
							break;	
				case "zipf":
								echo "x1: <input name=\"".$key."_x1\" type=\"text\" size=\"4\" value=\"".$params[$key."_x1"]."\"/> 
									x2: <input name=\"".$key."_x2\" type=\"text\" size=\"4\" value=\"".$params[$key."_x2"]."\"/> 
									p: <input name=\"".$key."_p\" type=\"text\" size=\"4\" value=\"".$params[$key."_p"]."\"/>";
								break;
								
				case "triangle":
									echo "a: <input name=\"".$key."_a\" type=\"text\" size=\"4\" value=\"".$params[$key."_a"]."\"/> 
									b: <input name=\"".$key."_b\" type=\"text\" size=\"4\" value=\"".$params[$key."_b"]."\"/> 
									c: <input name=\"".$key."_c\" type=\"text\" size=\"4\" value=\"".$params[$key."_c"]."\"/>";
									break;
			}
	}

	
?>
<div id="contenido">
	<div id="cabecera">
		<div id="logo"><img src="../img/cuenta_usuario.gif" alt="Cuenta Usuario" /></div>
	</div>
	<div id="central">
		<div id="primer_menu">
			<ul class="ul_primermenu">
				<li class="li_primermenu"><a href="../../index.php?section=1" class="link_primermenu">Home</a></li>
				<li class="li_primermenu"><a href="executions.php?logout=true" class="link_primermenu">Logout</a></li>
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
			<li class="pestana_selec"><strong>Executions</strong></li>
			<li class="pestanas"><a href="incidences.php">Incidences</a></li>
			<li class="pestanas"><strong><a href="preferences.php">Settings</a></strong></li>
		</ul>
	</div>
	<div id="parte_central">
	<div id="menu_interior">
		<ul class="ul_segundomenu">
			<li class="li_segundomenu"><a href="executions.php"> My executions</a></li>
			<li class="li_segundomenu"><span class="segundomenu_selec">New execution</span></li>
		</ul>
	</div>	 
<div id="result" align="center">
<h1>Game-based scheduler parameters</h1>
<div id="error" align="center">
	<?
		for($i = 0; $i < count($errors); $i++)
		{
			echo $errors[$i];
		}	
	?>
</div>
<form id="form1" name="form1" method="post" action="SG_Scheduler.php">
  <table width="80%" cellpadding="0" cellspacing="0" class="tabladatos">
     
    <tr>
    	<th class="th_tabladatos" scope="row"><strong>Game scenario</strong></th>
    	<td class="td_tabladatos_PAR" colspan="2">
    		<select name="--game_scenario" id="--game_scenario" onchange="loadProperties('leader_task', this)" >
    		<? 
    			if(isset($_POST))
    				getOptions(array("symmetric", "stackelberg"), array("symmetric", "stackelberg"), $_POST["--game_scenario"]);
    			else
    				getOptions(array("symmetric", "stackelberg"), array("symmetric", "stackelberg"), $params["--game_scenario"]);
    		?>
    		</select>
    	</td>
    	<td class="td_tabladatos_PAR" colspan="2"></td>
    </tr>
    <tr>
    	<th class="th_tabladatos" scope="row"><strong>Number of players</strong></th>
    	<td class="td_tabladatos_PAR" colspan="2">
    		<input name="--number_players" type="text" id="--number_players" size="4" 
    			value="<?if(isset($_POST["--number_players"])) 
    					echo $_POST["--number_players"];
    				else
    					echo $params["--number_players"];?>" />
    	</td>
    	<td class="td_tabladatos_PAR" colspan="2"></td>
    </tr>
     <tr>
      <th class="th_tabladatos" scope="row"><strong>Leader's task pool fraction</strong></td>
      <td class="td_tabladatos_PAR" colspan="2">	
      <div id="leader_task">
      	Enable only in stackelberg mode
	</div>
      
      </td>
	  <td class="td_tabladatos_PAR" colspan="2">
	  <div id="leader_task2">
	  </div>
    </tr>
    <tr>
      	<th class="th_tabladatos" scope="row" colspan="4">
      		<strong>Player's cost functions specification</strong>
     	</th>     	
   </tr> 
   <tr>
      <th class="th_tabladatos" scope="row"><strong>Execution cost</strong></th>
      <td class="td_tabladatos_PAR" colspan="3">
      	<select name="--execution_cost" id="--execution_cost" >
      	<? if(isset($_POST["--execution_cost"]))
					getOptions(array("enable", "disable"), array("enable", "disable"), $_POST["--execution_cost"]);
				else			
		  			getOptions(array("enable", "disable"), array("enable", "disable"), $params["--execution_cost"]);?>
		  			</select>
	   </td>
		  			<td class="td_tabladatos_PAR">
		  			</td>
    </tr>
   <tr>
      <th class="th_tabladatos" scope="row"><strong>Resource utilization cost</strong></th>
      <td class="td_tabladatos_PAR" colspan="3"><select name="--resource_utilization_cost" id="--resource_utilization_cost" >
      	<? if(isset($_POST["--resource_utilization_cost"]))
					getOptions(array("enable", "disable"), array("enable", "disable"), $_POST["--resource_utilization_cost"]);
				else			
		  			getOptions(array("enable", "disable"), array("enable", "disable"), $params["--resource_utilization_cost"]);?>
		  			</select>
        </td>
    </tr>
   <tr>
      <th class="th_tabladatos" scope="row"><strong>Security cost</strong></th>
      <td class="td_tabladatos_PAR" colspan="3"><select name="--security_cost" id="--security_cost" >
      	<? if(isset($_POST["--security_cost"]))
					getOptions(array("enable", "disable"), array("enable", "disable"), $_POST["--security_cost"]);
				else			
		  			getOptions(array("enable", "disable"), array("enable", "disable"), $params["--security_cost"]);?>
		  			</select> </td>
    </tr>
    <tr>
      <th class="th_tabladatos" scope="row"><strong>Task abortion cost </strong></td>
      <td class="td_tabladatos_PAR" colspan="3">
      	<select name="--task_abortion_cost" id="--task_abortion_cost" >
      	<? if(isset($_POST["--task_abortion_cost"]))
					getOptions(array("enable", "disable"), array("enable", "disable"), $_POST["--task_abortion_cost"]);
				else			
		  			getOptions(array("enable", "disable"), array("enable", "disable"), $params["--task_abortion_cost"]);?>
		  			</select>
		</td>
    </tr>
    <tr>
      <th class="th_tabladatos" scope="row"><strong>Scheduler at the global level</strong></th>
      <td class="td_tabladatos_PAR" colspan="3">
      	<input name="group1" id="--scheduler_global_level" type="radio" checked="checked" value="GA">Genetic Algorithm</input> 
        </td>
    </tr>
    
    <tr>
      <th class="th_tabladatos" scope="row"><strong>Scheduler at the player's level</strong></th>
      <td class="td_tabladatos_PAR" colspan="3">
      <input name="group2" type="radio" id="--scheduler_player_level" checked="checked" value="PMCT">PMCT</input>
    </tr>
  </table>
  <br />
  <input type="submit" name="Submit" value="Next" />
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
