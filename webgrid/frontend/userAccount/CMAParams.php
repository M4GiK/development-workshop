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
	$params = $cm->getDefaultParamsWithParametro("default_params_CMA");
	
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
				//ob_start();
				$strparams = "";
                                foreach($_POST as $param => $value)
                                {
                                        if($value!="" || $value=="0")
                                        {
                                                switch($param)
                                                {
                                                        case "--max_time_to_spend":
                                                        case "--opt_strategy":
                                                                $strparams .= "$param $value ";
                                                                $strparams = str_replace("CMA_scheduler", "CMA_scheduler". getGeneticAlgorithmParameters($_POST), $old_params);
                                                                break;
                                                }
                                        }
                                }
                                foreach($_POST as $param => $value)
                                {
                                        if($value!="" || $value=="0")
                                        {
                                                switch($param)
                                                {
                                                        case "--cma_nb_generations":
                                                                $strparams .= "--cma_nb_generations ".$value." ";
                                                                break;
                                                        case "--cma_population_height":
                                                                $strparams .= "--cma_population_height ".$value." ";
                                                                break;
                                                        case "--cma_population_width":
                                                                $strparams .= "--cma_population_width ".$value." ";
                                                                break;
                                                        case "--cma_nb_solutions_to_recombine":
                                                                $strparams .= "--cma_nb_solutions_to_recombine ".$value." ";
                                                                break;
                                                        case "--cma_nb_recombinations":
                                                                $strparams .= "--cma_nb_recombinations ".$value." ";
                                                                break;
                                                        case "--cma_nb_mutations":
                                                                $strparams .= "--cma_nb_mutations ".$value." ";
                                                                break;
                                                        case "--cma_start_choice":
                                                                $strparams .= "--cma_start_choice ".ConvertEnumToInt($value)." ";
                                                                break;
                                                        case "--cma_neighborhood_pattern":
                                                                $strparams .= "--cma_neighborhood_pattern ".$value." ";
                                                                break;
                                                        case "--cma_recombination_order":
                                                                $strparams .= "--cma_recombination_order ".ConvertEnumToInt($value)." ";
                                                                break;
                                                        case "--cma_mutation_order":
                                                                $strparams .= "--cma_mutation_order ".ConvertEnumToInt($value)." ";
                                                                break;
                                                        case "--cma_recombine_choice":
                                                                $strparams .= "--cma_recombine_choice ".ConvertEnumToInt($value)." ";
                                                                break;
                                                        case "--cma_recombine_selection":
                                                                $strparams .= "--cma_recombine_selection ".ConvertEnumToInt($value)." ";
                                                                break;
                                                        case "--cma_rec_selection_extra":
                                                                $strparams .= "--cma_rec_selection_extra ".$value." ";
                                                                break;
                                                        case "--cma_mutate_choice":
                                                                $strparams .= "--cma_mutate_choice ".ConvertEnumToInt($value)." ";
                                                                break;
                                                        case "--cma_mutate_extra_parameter":
                                                                $strparams .= "--cma_mutate_extra_parameter ".$value." ";
                                                                break;
                                                        case "--cma_nb_ls_not_improving_iterations":
                                                                $strparams .= "--cma_nb_ls_not_improving_iterations ".$value." ";
                                                                break;
                                                        case "--cma_add_only_if_better":
                                                                if($value == true)
                                                                        $strparams .= "--cma_add_only_if_better 1 ";
                                                                else
                                                                        $strparams .= "--cma_add_only_if_better 0 ";
                                                                break;
                                                }
                                        }
                                }
                                //echo $strparams;
                                setcookie('CMA_params', $strparams, time() + 3600);
                                //header("location:wait.php");
                                generateExecution($_SESSION["login"]["username"],$_POST, $strparams, 6);
				
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
				//ob_end_flush();
					
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
<h1>CMA parameters</h1>
<div id="error" align="center">
	<?
		for($i = 0; $i < count($errors); $i++)
		{
			echo $errors[$i];
		}	
	?>
</div>



<form id="form1" name="form1" method="post" action="CMAParams.php">



  <table width="80%" cellpadding="0" cellspacing="0" class="tabladatos">


    <tr>
      <th class="th_tabladatos" scope="row"><strong>Max time to spend</strong></td>
      <td class="td_tabladatos_PAR" colspan="2"><input name="--max_time_to_spend" type="text" id="--max_time_to_spend" size="4" value="<? if(isset($_POST["--max_time_to_spend"]))
				echo $_POST["--max_time_to_spend"];
			else
	  			echo $params["--max_time_to_spend"];
	  ?>" /></td>
	  <td class="td_tabladatos_PAR" colspan="2">
        (integer) (min:<? echo $params["min_--max_time_to_spend"]; ?>s)</td>
    </tr>


     <tr>
      	<th class="th_tabladatos" scope="row">
      		<strong>Optimization strategy</strong>
     	</th>
      	<td class="td_tabladatos_PAR" colspan="2">
      		<select name="--opt_strategy" id="--opt_strategy" onchange="loadProperties('params_workload',this)";>

			<?
				if(isset($_POST["--opt_strategy"]))
					getOptions(array("true", "false"),array("1", "0"),$_POST["--opt_strategy"]);

				else
					getOptions(array("true", "false"),array("1", "0"),$params["--opt_strategy"]);
			?>
						</select>
		</td>
		<td class="td_tabladatos_PAR" colspan="2">true - hierarchic<br/> false - simultaneous</td>
    </tr>


    <tr>
    	<th class="th_tabladatos" scope="row"><strong>Number of generations</strong></th>
    	<td class="td_tabladatos_PAR" colspan="2">
    		<input name="--cma_nb_generations" type="text" id="--cma_nb_generations" size="4"
    			value="<?if(isset($_POST["--cma_nb_generations"]))
    					echo $_POST["--cma_nb_generations"];
    				else
    					echo $params["--cma_nb_generations"];?>" />
    	</td>
    	<td class="td_tabladatos_PAR" colspan="2"></td>
    </tr>


    <tr>
    	<th class="th_tabladatos" scope="row"><strong>Population height</strong></th>
    	<td class="td_tabladatos_PAR" colspan="2">
    		<input name="--cma_population_height" type="text" id="--cma_population_height" size="4"
    			value="<? if(isset($_POST["--cma_population_height"]))
    					echo $_POST["--cma_population_height"];
    				else
    					echo $params["--cma_population_height"];?>" />
    	
    	</td>
    	<td class="td_tabladatos_PAR" colspan="2"></td>
    </tr>


    <tr>
      <th class="th_tabladatos" scope="row"><strong>Population width</strong></th>
      <td class="td_tabladatos_PAR" colspan="2"><input name="--cma_population_width" type="text" id="--cma_population_width" size="4" 
                        value="<? if(isset($_POST["--cma_population_width"]))
                                    echo $_POST["--cma_population_width"];
                            else
                                    echo $params["--cma_population_width"];
	  ?>" /></td>
	  <td class="td_tabladatos_PAR" colspan="2"></td>
    </tr>


    <tr>
      <th class="th_tabladatos" scope="row"><strong>Number of solutions to recombine</strong></th>
      <td class="td_tabladatos_PAR" colspan="2"><input name="--cma_nb_solutions_to_recombine" type="text" id="--cma_nb_solutions_to_recombine" size="4"
                        value="<? if(isset($_POST["--cma_nb_solutions_to_recombine"]))
                                    echo $_POST["--cma_nb_solutions_to_recombine"];
                            else
                                    echo $params["--cma_nb_solutions_to_recombine"];
	  ?>" /></td>
	  <td class="td_tabladatos_PAR" colspan="2"></td>
    </tr>


   <tr>
      <th class="th_tabladatos" scope="row"><strong>Number of recombinations</strong></th>
      <td class="td_tabladatos_PAR" colspan="2">
      	<input name="--cma_nb_recombinations" id="--cma_nb_recombinations" type="text" size="4" value="<? if(isset($_POST["--cma_nb_recombinations"]))
					echo $_POST["--cma_nb_recombinations"];
				else			
		  			echo $params["--cma_nb_recombinations"];
        ?>" /></td>
        <td class="td_tabladatos_PAR" colspan="2"></td>
   </tr>


   <tr>
      <th class="th_tabladatos" scope="row"><strong>Number of mutations</strong></th>
      <td class="td_tabladatos_PAR" colspan="2"><input name="--cma_nb_mutations" type="text" id="--cma_nb_mutations" size="4"
                        value="<? if(isset($_POST["--cma_nb_mutations"]))
                                    echo $_POST["--cma_nb_mutations"];
                            else
                                    echo $params["--cma_nb_mutations"];
	  ?>" /></td>
	  <td class="td_tabladatos_PAR" colspan="2"></td>
    </tr>


   <tr>
      <th class="th_tabladatos" scope="row"><strong>Start choice</strong></th>
      <td class="td_tabladatos_PAR" colspan="2"><select name="--cma_start_choice" id="--start_choice" >
	  	<?
			if(isset($_POST["--cma_start_choice"]))
				getOptions(array("StartLJFRSJFR", "StartMCT", "StartRandom", "StartAll",
                        "StartMinMin", "StartRelativeCost", "StartKPB", "StartAll2"), array("StartLJFRSJFR", "StartMCT", "StartRandom", "StartAll",
                        "StartMinMin", "StartRelativeCost", "StartKPB", "StartAll2"),$_POST["--cma_start_choice"]);
			else
				getOptions(array("StartLJFRSJFR", "StartMCT", "StartRandom", "StartAll",
                        "StartMinMin", "StartRelativeCost", "StartKPB", "StartAll2"), array("StartLJFRSJFR", "StartMCT", "StartRandom", "StartAll",
                        "StartMinMin", "StartRelativeCost", "StartKPB", "StartAll2"),$params["--cma_start_choice"]);
		?>
	  </select></td>
            <td class="td_tabladatos_PAR" colspan="2"></td>
    </tr>


    <tr>
      <th class="th_tabladatos" scope="row"><strong>Neightborhood pattern</strong></th>
      <td class="td_tabladatos_PAR" colspan="2">
      	<input name="--cma_neighborhood_pattern" id="--cma_neighborhood_pattern" type="text" size="4" value="<? if(isset($_POST["--cma_neighborhood_pattern"]))
							echo $_POST["--cma_neighborhood_pattern"];
						else
							echo $params["--cma_neighborhood_pattern"];?>" />
		</td>
		<td class="td_tabladatos_PAR"></td>
    </tr>


    <tr>
      <th class="th_tabladatos" scope="row"><strong>Recombination order</strong></th>
      <td class="td_tabladatos_PAR" colspan="2">
          <select name="--cma_recombination_order" id="--cma_recombination_order" >
	  	<?
			if(isset($_POST["--cma_recombination_order"]))
				getOptions(array("FixedLineSweep", "FixedRandomSweep", "NewRandomSweep"),
                                        array("FixedLineSweep", "FixedRandomSweep", "NewRandomSweep"),$_POST["--cma_recombination_order"]);
			else
				getOptions(array("FixedLineSweep", "FixedRandomSweep", "NewRandomSweep"),
                                        array("FixedLineSweep", "FixedRandomSweep", "NewRandomSweep"),$params["--cma_recombination_order"]);
		?>
	  </select></td>
        <td class="td_tabladatos_PAR" colspan="2">
        </td>
    </tr>

    
    <tr>
      <th class="th_tabladatos" scope="row"><strong>Mutation order</strong></th>
      <td class="td_tabladatos_PAR" colspan="2">
          <select name="--cma_mutation_order" id="--cma_mutation_order" >
	  	<?
			if(isset($_POST["--cma_mutation_order"]))
				getOptions(array("FixedLineSweep", "FixedRandomSweep", "NewRandomSweep"),
                                        array("FixedLineSweep", "FixedRandomSweep", "NewRandomSweep"),$_POST["--cma_mutation_order"]);
			else
				getOptions(array("FixedLineSweep", "FixedRandomSweep", "NewRandomSweep"),
                                        array("FixedLineSweep", "FixedRandomSweep", "NewRandomSweep"),$params["--cma_mutation_order"]);
		?>
	  </select></td>
            <td class="td_tabladatos_PAR" colspan="2"></td>
    </tr>


    <tr>
      <th class="th_tabladatos" scope="row"><strong>Recombine choice</strong></th>
      <td class="td_tabladatos_PAR" colspan="2">
          <select name="--cma_recombine_choice" id="--cma_recombine_choice" >
	  	<?
			if(isset($_POST["--cma_recombine_choice"]))
				getOptions(array("RecombineOnePoint", "RecombineUniform", "RecombineFitnessBased"),
                                        array("RecombineOnePoint", "RecombineUniform", "RecombineFitnessBased"),$_POST["--cma_recombine_choice"]);
			else
				getOptions(array("RecombineOnePoint", "RecombineUniform", "RecombineFitnessBased"),
                                        array("RecombineOnePoint", "RecombineUniform", "RecombineFitnessBased"),$params["--cma_recombine_choice"]);
		?>
	  </select></td>
		<td class="td_tabladatos_PAR" ></td>
   </tr>


   <tr>
      <th class="th_tabladatos" scope="row"><strong>Recombine selection</strong></th>
      <td class="td_tabladatos_PAR" colspan="2">
          <select name="--cma_recombine_choice" id="--cma_recombine_selection" >
	  	<?
			if(isset($_POST["--cma_recombine_selection"]))
				getOptions(array("SelectRandom", "SelectBest", "SelectLinearRanking", "SelectExpRanking", "SelectBinaryTournament",
                         "SelectNTournament"),array("SelectRandom", "SelectBest", "SelectLinearRanking", "SelectExpRanking", "SelectBinaryTournament",
                         "SelectNTournament"),$_POST["--cma_recombine_selection"]);
			else
				getOptions(array("SelectRandom", "SelectBest", "SelectLinearRanking", "SelectExpRanking", "SelectBinaryTournament",
                         "SelectNTournament"),array("SelectRandom", "SelectBest", "SelectLinearRanking", "SelectExpRanking", "SelectBinaryTournament",
                         "SelectNTournament"),$params["--cma_recombine_selection"]);
		?>
	  </select></td>
        <td class="td_tabladatos_PAR" colspan="2">
        </td>
    </tr>


    <tr>
      <th class="th_tabladatos" scope="row"><strong>Recombine selection extra</strong></th>
      <td class="td_tabladatos_PAR" colspan="2"><input name="--cma_rec_selection_extra" type="text" id="--cma_rec_selection_extra" size="4" value="<?
	  	if(isset($_POST["--cma_rec_selection_extra"]))
	  		echo $_POST["--cma_rec_selection_extra"];
		else
			echo $params["--cma_rec_selection_extra"];?>"/> </td>
            <td class="td_tabladatos_PAR" colspan="2"></td>
    </tr>
      
    
    <tr>
      <th class="th_tabladatos" scope="row"><strong>Mutate choice</strong></th>
      <td class="td_tabladatos_PAR" colspan="2"><select name="--cma_mutate_choice" id="--cma_mutate_choice" >
	  	<?
			if(isset($_POST["--cma_mutate_choice"]))
				getOptions(array("MutateMove", "MutateSwap", "MutateBoth",
    					 "MutateRebalancing"), array("MutateMove", "MutateSwap", "MutateBoth",
    					 "MutateRebalancing"),$_POST["--cma_mutate_choice"]);
			else
				getOptions(array("MutateMove", "MutateSwap", "MutateBoth",
    					 "MutateRebalancing"),array("MutateMove", "MutateSwap", "MutateBoth",
    					 "MutateRebalancing"),$params["--cma_mutate_choice"]);
		?>
	  </select>
        </td>
            <td class="td_tabladatos_PAR" colspan="2"></td>
    </tr>


    <tr>
      <th class="th_tabladatos" scope="row"><strong>Mutate extra parameter</strong></th>
      <td class="td_tabladatos_PAR" colspan="2"><input name="--cma_mutate_extra_parameter" type="text" id="--cma_mutate_extra_parameter" size="4" value="<?
	  	if(isset($_POST["--cma_mutate_extra_parameter"]))
	  		echo $_POST["--cma_mutate_extra_parameter"];
		else
			echo $params["--cma_mutate_extra_parameter"];?>"/> </td>
            <td class="td_tabladatos_PAR" colspan="2"></td>
    </tr>


    <tr>
      <th class="th_tabladatos" scope="row"><strong>cma_nb_ls_not_improving_iterations</strong></th>
      <td class="td_tabladatos_PAR" colspan="2"><input name="--cma_nb_ls_not_improving_iterations" type="text" id="--cma_nb_ls_not_improving_iterations" size="4" value="<?
	  	if(isset($_POST["--cma_nb_ls_not_improving_iterations"]))
	  		echo $_POST["--cma_nb_ls_not_improving_iterations"];
		else
			echo $params["--cma_nb_ls_not_improving_iterations"];?>"/> </td>
            <td class="td_tabladatos_PAR" colspan="2"></td>
    </tr>


    <tr>
      <th class="th_tabladatos" scope="row"><strong>Add only if better</strong></th>
      <td class="td_tabladatos_PAR" colspan="2">
      		<select name="--cma_add_only_if_better" id="--cma_add_only_if_better">

			<?
				if(isset($_POST["--cma_add_only_if_better"]))
					getOptions(array("false", "true"),array("0", "1"),$_POST["--cma_add_only_if_better"]);

				else
					getOptions(array("false", "true"),array("0", "1"),$params["--cma_add_only_if_better"]);
			?>
						</select></td>
            <td class="td_tabladatos_PAR" colspan="2"></td>
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
