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
				//case "--max_time_to_spend":			
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
		}
		if($default["max_--max_time_to_spend"] < $params["--max_time_to_spend"] || $default["min_--max_time_to_spend"] > $params["--max_time_to_spend"])
		{
			$errors[] = "<p align=\"left\"><img src=\"../../frontend/img/icons/exclamation.png\"/>&nbsp; The maximum time to spend should be between min and max value<p/>";				
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
	$params = $cm->getDefaultParamsWithParametro("default_params_TS");
	
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
				else
				{
					header("location:params.php");
				}
				
				$str_params = "";
				
				foreach($_POST as $param => $value)
				{
					switch($param)
					{
						case "--tb_nb_iterations":
							$str_params .= $param." ".$value." ";
							break;
						case "--tb_start_choice":
							$str_params .= $param." ".ConvertEnumToInt($value)." ";
							break;
						case "--tb_ser_use_delta_function":
							$str_params .= $param." ".$value." ";
							break;
						case "--tb_tabu_size":
							$str_params .= $param." ".$value." ";
							break;
						case "--tb_max_tabu_status":
							$str_params .= $param." ".$value." ";
							break;
						case "--tb_min_tabu_status":
							$str_params .= $param." ".$value." ";
							break;
						case "--tb_max_repetitions":
							$str_params .= $param." ".$value." ";
							break;
						case "--tb_nb_intensifications":
							$str_params .= $param." ".$value." ";
							break;
						case "--tb_nb_diversifications":
							$str_params .= $param." ".$value." ";
							break;
						case "--tb_elite_size":
							$str_params .= $param." ".$value." ";
							break;
						case "--tb_aspiration_value":
							$str_params .= $param." ".$value." ";
							break;
						case "--tb_max_nb_swaps":
							$str_params .= $param." ".$value." ";
							break;
						case "--tb_max_nb_transfers":
							$str_params .= $param." ".$value." ";
							break;
						case "--max_time_to_spend":
//						case "--opt_strategy":
								$old_params = str_replace("TS_scheduler", "TS_scheduler".getGeneticAlgorithmParameters($_POST), $old_params);
							break;
						default:
							break;
					}
				}
				//echo $old_params." ".$str_params;
				ob_start();
				generateExecution($_SESSION["login"]["username"],$_POST, $old_params." ".$str_params, 2);
				/*$avisos = $cm->getAvisos();
				if($avisos["ejecucion"] == "S") 
				{
					$to = $cm->getMail();
					$message = "El usuario:<strong> ".$_SESSION["login"]["username"]."</strong> ha realizado una ejecucion<br><br> Administracion WEBGRID"; 
					$alt_message = "El usuario:".$_SESSION["login"]["username"]."</strong> ha realizado una nueva ejecucion\n\n Administracion WEBGRID"; 
					$mail = new Mail("WEB GRID@UPC",$to,"Aviso: nueva ejecucion",$message,$alt_message, "../../lib/phpmailer/");
					$mail->send();
				}*/
				header("location:message_params.php?message=ok");
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
<h1>Tabu Search parameters</h1>
<div id="error" align="center">
	<?
		for($i = 0; $i < count($errors); $i++)
		{
			echo $errors[$i];
		}	
	?>
</div>
<form id="form1" name="form1" method="post" action="TabuSearch.php">
  <table width="80%" cellpadding="0" cellspacing="0" class="tabladatos">
  <tr>
  	<th class="th_tabladatos" scope="row">Number iterations</th>
  	<td class="td_tabladatos_PAR" colspan="4">
  		<input name="--tb_nb_iterations" id="--tb_nb_iterations" type="text" size="4"
  			value="<?if(isset($_POST["--tb_nb_iterations"]))
  					echo $_POST["--tb_nb_iterations"];
  					else
  					echo $params["--tb_nb_iterations"];
  				?>"/>
  	</td>
  </tr>
	<tr>
		<th class="th_tabladatos" scope="row">Start choice</th>
		<td class="td_tabladatos_PAR" colspan="4">
			<select name="--tb_start_choice" id="--tb_start_choice">
				<?
					if(isset($_POST["--tb_start_choice"]))
				getOptions(array("StartLJFRSJFR", "StartMCT", "StartRandom", "StartAll",
                        "StartMinMin", "StartRelativeCost", "StartKPB", "StartAll2"), array("StartLJFRSJFR", "StartMCT", "StartRandom", "StartAll",
                        "StartMinMin", "StartRelativeCost", "StartKPB", "StartAll2"),$_POST["--tb_start_choice"]);
			else
				getOptions(array("StartLJFRSJFR", "StartMCT", "StartRandom", "StartAll",
                        "StartMinMin", "StartRelativeCost", "StartKPB", "StartAll2"), array("StartLJFRSJFR", "StartMCT", "StartRandom", "StartAll",
                        "StartMinMin", "StartRelativeCost", "StartKPB", "StartAll2"),$params["--tb_start_choice"]);
				 ?>
			</select>
		</td>
	</tr>
	<tr>
	<th class="th_tabladatos" scope="row">Use delta function</th>
	<td class="td_tabladatos_PAR" colspan="4">
		<select name="--tb_set_use_delta_function" id="--tb_set_use_delta_function">
		<?
			if(isset($_POST["--tb_ser_use_delta_function"]))
			getOptions(array("true", "false"), array("1", "0"), $_POST["--tb_set_use_delta_function"]);
			else
			getOptions(array("true", "false"), array("1", "0"), $params["--tb_set_use_delta_function"]);
		?>
		</select>
	</td>
	</tr>
	<tr>
		<th class="th_tabladatos" scope="row">Tabu size</th>
		<td class="td_tabladatos_PAR" colspan="4">
			<input name="--tb_tabu_size" id="--tb_tabu_size" type="text" size="4" value="<?if(isset($_POST["--tb_tabu_size"]))
				echo $_POST["--tb_tabu_size"];
				else
				echo $params["--tb_tabu_size"];?> "></input>
		</td>
	</tr>
	<tr>
    	<th class="th_tabladatos" scope="row">Max tabu status</th>
    	<td class="td_tabladatos_PAR" colspan="4">
    	<input name="--tb_max_tabu_status" id="--tb_max_tabu_status" type="text" size="4" value="<?if(isset($_POST["--tb_max_tabu_status"]))
    		echo $_POST["--tb_max_tabu_status"];
    		else
    		echo $params["--tb_max_tabu_status"]; ?>"/>
    		</td>
    </tr>
    <tr>
    	<th class="th_tabladatos" scope="row">Max tabu status</th>
    	<td class="td_tabladatos_PAR" colspan="4">
    	<input name="--tb_min_tabu_status" id="--tb_min_tabu_status" type="text" size="4" value="<?if(isset($_POST["--tb_min_tabu_status"]))
    		echo $_POST["--tb_min_tabu_status"];
    		else
    		echo $params["--tb_min_tabu_status"]; ?>"/>
    		</td>
    </tr>
    <tr>
    	<th class="th_tabladatos" scope="row">Max repetitions</th>
    	<td class="td_tabladatos_PAR" colspan="4">
    	<input name="--tb_max_repetitions" id="--tb_max_repetiotions" type="text" size="4"
    	value="<?if(isset($_POST["--tb_max_repetitions"]))
    		echo $_POST["--tb_max_repetitions"];
    		else
    		echo $params["--tb_max_repetitions"]; ?> " />
    </td>
    <tr>
    	<th class="th_tabladatos" scope="row">Number of intensifications</th>
    	<td class="td_tabladatos_PAR" colspan="4">
    	<input name="--tb_nb_intensifications" id="--tb_nb_intensifications" type="text" size="4"
    	value="<? if(isset($_POST["--tb_nb_intensifications"]))
    		echo $_POST["--tb_nb_intensifications"];
    		else
    		echo $params["--tb_nb_intensifications"];?>" />
    	</td>
    </tr>
    <tr>
    	<th class="th_tabladatos" scope="row">Number of diversifications</th>
    	<td class="td_tabladatos_PAR" colspan="4">
    		<input name="--tb_nb_diversifications" id="--tb_nb_diversifications" type="text" size="4"
    			value="<?if(isset($_POST["--tb_nb_diversifications"]))
    				echo $_POST["--tb_nb_diversifications"];
    				else
    				echo $params["--tb_nb_diversifications"]; ?>" />
    	</td>
    </tr>
    <tr>
    	<th class="th_tabladatos" scope="row">Elite size</th>
    	<td class="td_tabladatos_PAR" colspan="4">
    		<input name="--tb_elite_size" id="--tb_elite_size" type="text" size="4"
    			value="<? if(isset($_POST["--tb_elite_size"]))
    			echo $_POST["--tb_elite_size"];
    			else
    			echo $params["--tb_elite_size"]; ?>"/>
    	</td>
    </tr>
    <tr>
    	<th class="th_tabladatos" scope="row">Aspiration value</th>
    	<td class="td_tabladatos_PAR" colspan="4">
    		<input name="--tb_aspiration_value" id="--tb_aspiration_value" type="text" size="4"
    			value="<? if(isset($_POST["--tb_aspiration_value"]))
    				echo $_POST["--tb_aspiration_value"];
    				else
    				echo $params["--tb_aspiration_value"]; ?>"/>
    	</td>
    </tr>
    <tr>
    	<th class="th_tabladatos" scope="row">Max number swaps</th>
    	<td class="td_tabladatos_PAR" colspan="4">
    		<input name="--tb_max_nb_swaps" id="--tb_max_nb_swaps" type="text" size="4"
    			value="<? if(isset($_POST["--tb_max_nb_swaps"]))
    				echo $_POST["--tb_max_nb_swaps"];
    				else
    				echo $params["--tb_max_nb_swaps"]; ?>"/>
    	</td>
    </tr>
    <tr>
    	<th class="th_tabladatos" scope="row">Max number transfers</th>
    	<td class="td_tabladatos_PAR" colspan="4">
    		<input name="--tb_max_nb_transfers" id="--tb_max_nb_transfers" type="text" size="4"
    			value="<? if(isset($_POST["--tb_max_nb_transfers"]))
    				echo $_POST["--tb_max_nb_transfers"];
    				else
    				echo $params["--tb_max_nb_transfers"]; ?>"/>
    	</td>
    </tr>
    <tr>
    	<th class="th_tabladatos" scope="row">Aspiration value</th>
    	<td class="td_tabladatos_PAR" colspan="4">
    		<input name="--tb_aspiration_value" id="--tb_aspiration_value" type="text" size="4"
    			value="<? if(isset($_POST["--tb_aspiration_value"]))
    				echo $_POST["--tb_aspiration_value"];
    				else
    				echo $params["--tb_aspiration_value"]; ?>"/>
    	</td>
    </tr>
    <tr>
    	<th class="th_tabladatos" scope="row">Max time to spend</th>
    	<td class="td_tabladatos_PAR" colspan="2">
			<input name="--max_time_to_spend" id="--max_time_to_spend" type="text" size ="4"
			value="<?if(isset($_POST["--max_time_to_spend"])) 
						echo $_POST["--max_time_to_spend"];
						else
						echo $params["--max_time_to_spend"];?>"/>
    	</td>
    	<td class="td_tabladatos_PAR" colspan="2">
    		(min: <? echo $params["min_--max_time_to_spend"]; ?>, max: <? echo $params["max_--max_time_to_spend"]; ?>)
    	</td>
    </tr>
    <tr>
    	<th class="th_tabladatos" scope="row">Optimatization strategy</th>
    	<td class="td_tabladatos_PAR" colspan="4">
    		<select name="--opt_strategy" id="--opt_strategy">
    			<?
    				if(isset($_POST))
    					getOptions(array("true", "false"), array("true", "false"), $_POST["--opt_strategy"]);
    				else
    					getOptions(array("true", "false"), array("true", "false"), $params["--opt_strategy"]);
    			?>
    		</select>
    	</td>
    </tr>
  </table>
  <br />
  <input type="submit" name="Submit" value="Run" />
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
