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
	$dic_params = array( "--ttasks" => "Total number of tasks",
						"--iatime" => "Task arrival distribution", 
						"--workload" =>"Workload distribution",
						"--ihosts" => "Initial number of machines",
						"--mips"=>"Mips distribution", 
						"--addhost" =>"Distribution of machines joining the system",
						"--delhost" => "Distribution of machines dropping from the system",
						"--maxhosts" =>"Maximum number of machines in the system",
						"--hostselect" => "Resource selection policy", 
						"--activate" =>"Policy of scheduler activation",
						"--allocpolicy" => "Local policy for managing tasks at host", 
						"--nruns" => "Independent runs",
						"--strategy" => "Strategy",
						"--seed"=>"Seed",
                                                "--security"=>"Security",
                                                "--security_param"=>"Security parameter",
                                                "--energy"=>"Energy",
						"--itasks" => "Initial number of tasks",
						"--minhosts" => "Minimum number of machines in the system");
	
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
				case "--ttasks":
									
				case "--itasks":
									
				case "--ihosts":
									
				case "--minhosts": 
									
				case "--maxhosts":
				case "--nruns":
								if(!is_numeric($value))
									$errors[] = "<p align=\"left\"><img src=\"../../frontend/img/icons/exclamation.png\"/>&nbsp; $dic_params[$key]  	value should be numeric <p/>";				
								else
								{
									$params[$key] = intval($value);
									if($default["max_".$key] < $value || $default["min_".$key] > $value)
										$errors[] = "<p align=\"left\"><img src=\"../../frontend/img/icons/exclamation.png\"/>&nbsp; $dic_params[$key]  	must be in the interval: [".$default["min_".$key]." - ".$default["max_".$key]."]<p/>";
								
								}							
								
								
								break;
                                case "--security_param":
                                                                if(!is_numeric($value))
									$errors[] = "<p align=\"left\"><img src=\"../../frontend/img/icons/exclamation.png\"/>&nbsp; $dic_params[$key]  	value should be numeric <p/>";
								else
								{
									$params[$key] = $value;
									if($default["max_".$key] < $value || $default["min_".$key] > $value)
										$errors[] = "<p align=\"left\"><img src=\"../../frontend/img/icons/exclamation.png\"/>&nbsp; $dic_params[$key]  	must be in the interval: [".$default["min_".$key]." - ".$default["max_".$key]."]<p/>";

								}


								break;
			}
		}
		
		if($params["--ttasks"] < $params["--itasks"])
		{
			$errors[] = "<p align=\"left\"><img src=\"../../frontend/img/icons/exclamation.png\"/>&nbsp; The number of initial tasks should be less than the total number of tasks in the system <p/>";				
		}
		
		if($params["--minhosts"] > $params["--maxhosts"])
		{
			$errors[] = "<p align=\"left\"><img src=\"../../frontend/img/icons/exclamation.png\"/>&nbsp; The minimum number of hosts should be less than the maximum number <p/>";				
		}
		
		
		if($params["min_--ihosts"] > $POST["--ihosts"] || $params["max_--ihosts"] < $POST["--ihosts"] )
		{
			$errors[] = "<p align=\"left\"><img src=\"../../frontend/img/icons/exclamation.png\"/>&nbsp; The number of initial hosts should be between the min and max number of hosts <p/>";				
		}
		if($params["--ihosts"] < $params["--minhosts"] || $params["--ihosts"] > $params["--maxhosts"])
		{
			$errors[] = "<p align=\"left\"><img src=\"../../frontend/img/icons/exclamation.png\"/>&nbsp; Change value of min and max host <p/>";				
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
		checkDistributionValues($params,$errors);
		return $errors;
	}

	function getParams($params)
{
	foreach($params as $param => $value)
	{
		if($value!="" || $value=="0")
		{
			switch($param)
			{
				case "--ttasks":
				case "--itasks":
				case "--ihosts":
				case "--minhosts":
				case "--maxhosts":
				case "--allocpolicy":
				case "--strategy":
				case "--nruns":
				case "--seed":
				case "--hostselect":	
					$strparams .= "$param $value ";
					break;				
				case "--iatime":
				case "--workload":
				case "--activate":
				case "--mips":
				case "--addhost":
				case "--delhost":
					$strparams.= generateDistibutionParam($params,$param,$value);
					break;
				case "--reschedule":
					if($value=="Y")
						$strparams.= "$param ";
					break;					
			}				
		}
	}
	return $strparams;
}	
	
	$cm = new configManager();
	$params = $cm->getDefaultParams();
	
	
	checkPermision($cm,$user);
	
	if(isset($_POST["Submit"]))
	{
			$errors = check($_POST,$params);
			
			if(count($errors) == 0)
			{
					switch($_POST["--strategy"])
					{
						case "GA_scheduler":
							$str_params = "";
							foreach($_POST as $param => $value)
							{
								//echo $param." ".$value."<br/>";
								//$str_params = getParams($_POST);								
								switch ($param)
								{
									case "--ttasks":
										$str_params .= "-n ".$value." ";
										break;
									case "--itasks":
										$str_params .= "-b ".$value." ";
										break;
									case "--iatime":
										$str_params .= "-i ".generateDistibutionParam($_POST, $param, $value)." ";
										break;
									case "--ihosts":
										$str_params .= "-o ".$value." ";
										break;
									case "--mips":
										$str_params .= "-m ".generateDistibutionParam($_POST, $param, $value)." ";
										break;
									case "--addhost":
										$str_params .= "-a ".generateDistibutionParam($_POST, $param, $value)." ";
										break;
									case "--delhost":
										$str_params .= "-d ".generateDistibutionParam($_POST, $param, $value)." ";
										break;
									case "--minhosts":
										$str_params .= "-f ".$value." ";
										break;
									case "--maxhosts":
										$str_params .= "-g ".$value." ";
										break;
									case "--reschedule":
										$str_params .= "-r ".$value." ";
										break;
									case "--strategy":
										$str_params .= "-s ".$value." ";
										break;
									case "--hostselect":
										$str_params .= "-x ".$value." ";
										break;
									case "--workload":
										$str_params .= "-w ".generateDistibutionParam($_POST, $param, $value)." ";
										break;
									case "--taskselect":
										$str_params .= "-y ".$value." ";
										break;
									case "--scheduler_activation":
										$str_params .= "-z ".$value." ";
										break;
									case "--allocpolicy":
										$str_params .= "-l ".$value." ";
										break;
									case "--nruns":
										$str_params .= "-1 ".$value." ";
										break;
									case "--seed":
										$str_params .= "-2 ".$value." ";
										break;
									case "--trace":
										$str_params .= "-t ".$value." ";
										break;
									case "--help":
										$str_params .= "-h ".$value." ";
										break;
                                                                        case "--security":
                                                                                $str_params .= "--security ".$value." ";
                                                                                break;
                                                                        case "--security_param":
                                                                                $str_params .= "--security_param ".$value." ";
                                                                                break;
                                                                        case "--energy":
                                                                                $str_params .= "--energy ".$value." ";
                                                                                break;
								}
							}
							//echo "<br/>".$str_params;
							setcookie('params', $str_params, time() + 3600);
							header("location:GeneticAlgorithmParams.php");
							break;
						case "HGS_scheduler":
					$str_params = "";
							foreach($_POST as $param => $value)
							{
								//echo $param." ".$value."<br/>";
								//$str_params = getParams($_POST);								
								switch ($param)
								{
									case "--ttasks":
										$str_params .= "-n ".$value." ";
										break;
									case "--itasks":
										$str_params .= "-b ".$value." ";
										break;
									case "--iatime":
										$str_params .= "-i ".generateDistibutionParam($_POST, $param, $value)." ";
										break;
									case "--ihosts":
										$str_params .= "-o ".$value." ";
										break;
									case "--mips":
										$str_params .= "-m ".generateDistibutionParam($_POST, $param, $value)." ";
										break;
									case "--addhost":
										$str_params .= "-a ".generateDistibutionParam($_POST, $param, $value)." ";
										break;
									case "--delhost":
										$str_params .= "-d ".generateDistibutionParam($_POST, $param, $value)." ";
										break;
									case "--minhosts":
										$str_params .= "-f ".$value." ";
										break;
									case "--maxhosts":
										$str_params .= "-g ".$value." ";
										break;
									case "--reschedule":
										$str_params .= "-r ".$value." ";
										break;
									case "--strategy":
										$str_params .= "-s ".$value." ";
										break;
									case "--hostselect":
										$str_params .= "-x ".$value." ";
										break;
									case "--workload":
										$str_params .= "-w ".generateDistibutionParam($_POST, $param, $value)." ";
										break;
									case "--taskselect":
										$str_params .= "-y ".$value." ";
										break;
									case "--scheduler_activation":
										$str_params .= "-z ".$value." ";
										break;
									case "--allocpolicy":
										$str_params .= "-l ".$value." ";
										break;
									case "--nruns":
										$str_params .= "-1 ".$value." ";
										break;
									case "--seed":
										$str_params .= "-2 ".$value." ";
										break;
									case "--trace":
										$str_params .= "-t ".$value." ";
										break;
									case "--help":
										$str_params .= "-h ".$value." ";
										break;
                                                                        case "--security":
                                                                                $str_params .= "--security ".$value." ";
                                                                                break;
                                                                        case "--security_param":
                                                                                $str_params .= "--security_param ".$value." ";
                                                                                break;
                                                                        case "--energy":
                                                                                $str_params .= "--energy ".$value." ";
                                                                                break;
								}
							}
							setcookie('params', $str_params, time() + 3600);
							header("location:HGSParams.php");
							break;
						case "TS_scheduler":
					$str_params = "";
							foreach($_POST as $param => $value)
							{
								//echo $param." ".$value."<br/>";
								//$str_params = getParams($_POST);								
								switch ($param)
								{
									case "--ttasks":
										$str_params .= "-n ".$value." ";
										break;
									case "--itasks":
										$str_params .= "-b ".$value." ";
										break;
									case "--iatime":
										$str_params .= "-i ".generateDistibutionParam($_POST, $param, $value)." ";
										break;
									case "--ihosts":
										$str_params .= "-o ".$value." ";
										break;
									case "--mips":
										$str_params .= "-m ".generateDistibutionParam($_POST, $param, $value)." ";
										break;
									case "--addhost":
										$str_params .= "-a ".generateDistibutionParam($_POST, $param, $value)." ";
										break;
									case "--delhost":
										$str_params .= "-d ".generateDistibutionParam($_POST, $param, $value)." ";
										break;
									case "--minhosts":
										$str_params .= "-f ".$value." ";
										break;
									case "--maxhosts":
										$str_params .= "-g ".$value." ";
										break;
									case "--reschedule":
										$str_params .= "-r ".$value." ";
										break;
									case "--strategy":
										$str_params .= "-s ".$value." ";
										break;
									case "--hostselect":
										$str_params .= "-x ".$value." ";
										break;
									case "--workload":
										$str_params .= "-w ".generateDistibutionParam($_POST, $param, $value)." ";
										break;
									case "--taskselect":
										$str_params .= "-y ".$value." ";
										break;
									case "--scheduler_activation":
										$str_params .= "-z ".$value." ";
										break;
									case "--allocpolicy":
										$str_params .= "-l ".$value." ";
										break;
									case "--nruns":
										$str_params .= "-1 ".$value." ";
										break;
									case "--seed":
										$str_params .= "-2 ".$value." ";
										break;
									case "--trace":
										$str_params .= "-t ".$value." ";
										break;
									case "--help":
										$str_params .= "-h ".$value." ";
										break;
                                                                        case "--security":
                                                                                $str_params .= "--security ".$value." ";
                                                                                break;
                                                                        case "--security_param":
                                                                                $str_params .= "--security_param ".$value." ";
                                                                                break;
                                                                        case "--energy":
                                                                                $str_params .= "--energy ".$value." ";
                                                                                break;
								}
							}
							setcookie('params', $str_params, time() + 3600);
							header("location:TabuSearch.php");
							//header("location:message_params.php?message=ok");
							break;
						case "SG_scheduler":
					$str_params = "";
							foreach($_POST as $param => $value)
							{
								//echo $param." ".$value."<br/>";
								//$str_params = getParams($_POST);								
								switch ($param)
								{
									case "--ttasks":
										$str_params .= "-n ".$value." ";
										break;
									case "--itasks":
										$str_params .= "-b ".$value." ";
										break;
									case "--iatime":
										$str_params .= "-i ".generateDistibutionParam($_POST, $param, $value)." ";
										break;
									case "--ihosts":
										$str_params .= "-o ".$value." ";
										break;
									case "--mips":
										$str_params .= "-m ".generateDistibutionParam($_POST, $param, $value)." ";
										break;
									case "--addhost":
										$str_params .= "-a ".generateDistibutionParam($_POST, $param, $value)." ";
										break;
									case "--delhost":
										$str_params .= "-d ".generateDistibutionParam($_POST, $param, $value)." ";
										break;
									case "--minhosts":
										$str_params .= "-f ".$value." ";
										break;
									case "--maxhosts":
										$str_params .= "-g ".$value." ";
										break;
									case "--reschedule":
										$str_params .= "-r ".$value." ";
										break;
									case "--strategy":
										$str_params .= "-s ".$value." ";
										break;
									case "--hostselect":
										$str_params .= "-x ".$value." ";
										break;
									case "--workload":
										$str_params .= "-w ".generateDistibutionParam($_POST, $param, $value)." ";
										break;
									case "--taskselect":
										$str_params .= "-y ".$value." ";
										break;
									case "--scheduler_activation":
										$str_params .= "-z ".$value." ";
										break;
									case "--allocpolicy":
										$str_params .= "-l ".$value." ";
										break;
									case "--nruns":
										$str_params .= "-1 ".$value." ";
										break;
									case "--seed":
										$str_params .= "-2 ".$value." ";
										break;
									case "--trace":
										$str_params .= "-t ".$value." ";
										break;
									case "--help":
										$str_params .= "-h ".$value." ";
										break;
                                                                        case "--energy":
                                                                                $str_params .= "--energy ".$value." ";
                                                                                break;
								}
							}
							setcookie('params', $str_params, time() + 3600);
							header("location:SG_Scheduler.php");
							break;
						case "AdHoc_scheduler":
							$str_params = "";
							foreach($_POST as $param => $value)
							{
								//echo $param." ".$value."<br/>";
								//$str_params = getParams($_POST);								
								switch ($param)
								{
									case "--ttasks":
										$str_params .= "-n ".$value." ";
										break;
									case "--itasks":
										$str_params .= "-b ".$value." ";
										break;
									case "--iatime":
										$str_params .= "-i ".generateDistibutionParam($_POST, $param, $value)." ";
										break;
									case "--ihosts":
										$str_params .= "-o ".$value." ";
										break;
									case "--mips":
										$str_params .= "-m ".generateDistibutionParam($_POST, $param, $value)." ";
										break;
									case "--addhost":
										$str_params .= "-a ".generateDistibutionParam($_POST, $param, $value)." ";
										break;
									case "--delhost":
										$str_params .= "-d ".generateDistibutionParam($_POST, $param, $value)." ";
										break;
									case "--minhosts":
										$str_params .= "-f ".$value." ";
										break;
									case "--maxhosts":
										$str_params .= "-g ".$value." ";
										break;
									case "--reschedule":
										$str_params .= "-r ".$value." ";
										break;
									case "--strategy":
										$str_params .= "-s ".$value."(".$_POST["--Adhoc_heuristic"].") ";
										break;
									case "--hostselect":
										$str_params .= "-x ".$value." ";
										break;
									case "--workload":
										$str_params .= "-w ".generateDistibutionParam($_POST, $param, $value)." ";
										break;
									case "--taskselect":
										$str_params .= "-y ".$value." ";
										break;
									case "--scheduler_activation":
										$str_params .= "-z ".$value." ";
										break;
									case "--allocpolicy":
										$str_params .= "-l ".$value." ";
										break;
									case "--nruns":
										$str_params .= "-1 ".$value." ";
										break;
									case "--seed":
										$str_params .= "-2 ".$value." ";
										break;
									case "--trace":
										$str_params .= "-t ".$value." ";
										break;
									case "--help":
										$str_params .= "-h ".$value." ";
										break;
                                                                        case "--energy":
                                                                                $str_params .= "--energy ".$value." ";
                                                                                break;
								}
							}
							generateExecution($_SESSION["login"]["username"],$_POST, $str_params, 4);
							header("location:message_params.php?message=ok");
							break;
                                                case "CMA_scheduler":
							$str_params = "";
							foreach($_POST as $param => $value)
							{
								//echo $param." ".$value."<br/>";
								//$str_params = getParams($_POST);
								switch ($param)
								{
									case "--ttasks":
										$str_params .= "-n ".$value." ";
										break;
									case "--itasks":
										$str_params .= "-b ".$value." ";
										break;
									case "--iatime":
										$str_params .= "-i ".generateDistibutionParam($_POST, $param, $value)." ";
										break;
									case "--ihosts":
										$str_params .= "-o ".$value." ";
										break;
									case "--mips":
										$str_params .= "-m ".generateDistibutionParam($_POST, $param, $value)." ";
										break;
									case "--addhost":
										$str_params .= "-a ".generateDistibutionParam($_POST, $param, $value)." ";
										break;
									case "--delhost":
										$str_params .= "-d ".generateDistibutionParam($_POST, $param, $value)." ";
										break;
									case "--minhosts":
										$str_params .= "-f ".$value." ";
										break;
									case "--maxhosts":
										$str_params .= "-g ".$value." ";
										break;
									case "--reschedule":
										$str_params .= "-r ".$value." ";
										break;
									case "--strategy":
										$str_params .= "-s ".$value." ";
										break;
									case "--hostselect":
										$str_params .= "-x ".$value." ";
										break;
									case "--workload":
										$str_params .= "-w ".generateDistibutionParam($_POST, $param, $value)." ";
										break;
									case "--taskselect":
										$str_params .= "-y ".$value." ";
										break;
									case "--scheduler_activation":
										$str_params .= "-z ".$value." ";
										break;
									case "--allocpolicy":
										$str_params .= "-l ".$value." ";
										break;
									case "--nruns":
										$str_params .= "-1 ".$value." ";
										break;
									case "--seed":
										$str_params .= "-2 ".$value." ";
										break;
									case "--trace":
										$str_params .= "-t ".$value." ";
										break;
									case "--help":
										$str_params .= "-h ".$value." ";
										break;
                                                                        case "--security":
                                                                                $str_params .= "--security ".$value." ";
                                                                                break;
                                                                        case "--security_param":
                                                                                $str_params .= "--security_param ".$value." ";
                                                                                break;
                                                                        case "--energy":
                                                                                $str_params .= "--energy ".$value." ";
                                                                                break;
								}
							}
							//echo "<br/>".$str_params;
							setcookie('params', $str_params, time() + 3600);
							header("location:CMAParams.php");
							break;
                                                case "MA_scheduler":
							$str_params = "";
							foreach($_POST as $param => $value)
							{
								//echo $param." ".$value."<br/>";
								//$str_params = getParams($_POST);
								switch ($param)
								{
									case "--ttasks":
										$str_params .= "-n ".$value." ";
										break;
									case "--itasks":
										$str_params .= "-b ".$value." ";
										break;
									case "--iatime":
										$str_params .= "-i ".generateDistibutionParam($_POST, $param, $value)." ";
										break;
									case "--ihosts":
										$str_params .= "-o ".$value." ";
										break;
									case "--mips":
										$str_params .= "-m ".generateDistibutionParam($_POST, $param, $value)." ";
										break;
									case "--addhost":
										$str_params .= "-a ".generateDistibutionParam($_POST, $param, $value)." ";
										break;
									case "--delhost":
										$str_params .= "-d ".generateDistibutionParam($_POST, $param, $value)." ";
										break;
									case "--minhosts":
										$str_params .= "-f ".$value." ";
										break;
									case "--maxhosts":
										$str_params .= "-g ".$value." ";
										break;
									case "--reschedule":
										$str_params .= "-r ".$value." ";
										break;
									case "--strategy":
										$str_params .= "-s ".$value." ";
										break;
									case "--hostselect":
										$str_params .= "-x ".$value." ";
										break;
									case "--workload":
										$str_params .= "-w ".generateDistibutionParam($_POST, $param, $value)." ";
										break;
									case "--taskselect":
										$str_params .= "-y ".$value." ";
										break;
									case "--scheduler_activation":
										$str_params .= "-z ".$value." ";
										break;
									case "--allocpolicy":
										$str_params .= "-l ".$value." ";
										break;
									case "--nruns":
										$str_params .= "-1 ".$value." ";
										break;
									case "--seed":
										$str_params .= "-2 ".$value." ";
										break;
									case "--trace":
										$str_params .= "-t ".$value." ";
										break;
									case "--help":
										$str_params .= "-h ".$value." ";
										break;
                                                                        case "--security":
                                                                                $str_params .= "--security ".$value." ";
                                                                                break;
                                                                        case "--security_param":
                                                                                $str_params .= "--security_param ".$value." ";
                                                                                break;
                                                                        case "--energy":
                                                                                $str_params .= "--energy ".$value." ";
                                                                                break;
								}
							}
							//echo "<br/>".$str_params;
							setcookie('params', $str_params, time() + 3600);
							header("location:MAParams.php");
							break;
                                                case "NGA_scheduler":
							$str_params = "";
							foreach($_POST as $param => $value)
							{
								//echo $param." ".$value."<br/>";
								//$str_params = getParams($_POST);
								switch ($param)
								{
									case "--ttasks":
										$str_params .= "-n ".$value." ";
										break;
									case "--itasks":
										$str_params .= "-b ".$value." ";
										break;
									case "--iatime":
										$str_params .= "-i ".generateDistibutionParam($_POST, $param, $value)." ";
										break;
									case "--ihosts":
										$str_params .= "-o ".$value." ";
										break;
									case "--mips":
										$str_params .= "-m ".generateDistibutionParam($_POST, $param, $value)." ";
										break;
									case "--addhost":
										$str_params .= "-a ".generateDistibutionParam($_POST, $param, $value)." ";
										break;
									case "--delhost":
										$str_params .= "-d ".generateDistibutionParam($_POST, $param, $value)." ";
										break;
									case "--minhosts":
										$str_params .= "-f ".$value." ";
										break;
									case "--maxhosts":
										$str_params .= "-g ".$value." ";
										break;
									case "--reschedule":
										$str_params .= "-r ".$value." ";
										break;
									case "--strategy":
										$str_params .= "-s ".$value." ";
										break;
									case "--hostselect":
										$str_params .= "-x ".$value." ";
										break;
									case "--workload":
										$str_params .= "-w ".generateDistibutionParam($_POST, $param, $value)." ";
										break;
									case "--taskselect":
										$str_params .= "-y ".$value." ";
										break;
									case "--scheduler_activation":
										$str_params .= "-z ".$value." ";
										break;
									case "--allocpolicy":
										$str_params .= "-l ".$value." ";
										break;
									case "--nruns":
										$str_params .= "-1 ".$value." ";
										break;
									case "--seed":
										$str_params .= "-2 ".$value." ";
										break;
									case "--trace":
										$str_params .= "-t ".$value." ";
										break;
									case "--help":
										$str_params .= "-h ".$value." ";
										break;
                                                                        case "--security":
                                                                                $str_params .= "--security ".$value." ";
                                                                                break;
                                                                        case "--security_param":
                                                                                $str_params .= "--security_param ".$value." ";
                                                                                break;
                                                                        case "--energy":
                                                                                $str_params .= "--energy ".$value." ";
                                                                                break;
								}
							}
							//echo "<br/>".$str_params;
							setcookie('params', $str_params, time() + 3600);
							header("location:NewGeneticAlgorithmParams.php");
							break;
						default:
							echo $_POST["--strategy"]."sdsd";
							//header("location:HGSParams.php");
							break;
					}
			
				//generateExecution($_SESSION["login"]["username"],$_POST);
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
function ActiveControl(name)
{
	if(name.value == "AdHoc_scheduler")
	{
		document.getElementById("--Adhoc_heuristic").style.display = 'inherit';
		div = document.getElementById("divAdhoc");
		html = "";
		div.innerHTML = html;
	}
	else
	{
		document.getElementById("--Adhoc_heuristic").style.display = 'none';
		div = document.getElementById("divAdhoc");
		html = "Enabled only when Adhoc selected";
		div.innerHTML = html;
	}

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
				case "n":
							echo "mean: <input name=\"".$key."_mean\" type=\"text\" size=\"4\" value=\"".$params[$key."_mean"]."\"/> 
									sd: <input name=\"".$key."_sd\"\" type=\"text\" size=\"4\" value=\"".$params[$key."_sd"]."\" />";
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


<h1>Simulator parameters</h1>
<div id="error" align="center">
	<?
		for($i = 0; $i < count($errors); $i++)
		{
			echo $errors[$i];
		}	
	?>
</div>
<form id="form1" name="form1" method="post" action="params.php">
  <table width="80%" cellpadding="0" cellspacing="0" class="tabladatos">
    <tr>
      <th class="th_tabladatos" scope="row"><strong>Total number of tasks </strong></td>
      <td class="td_tabladatos_PAR" colspan="2"><input name="--ttasks" type="text" id="--ttasks" size="4" value="<? if(isset($_POST["--ttasks"]))
				echo $_POST["--ttasks"];
			else
				echo $params["--ttasks"];?>"/>
        (integer)(min:<? echo $params["min_--ttasks"]; ?> max:<? echo $params["max_--ttasks"];?>)</td>
    </tr>
    <tr>
      <th class="th_tabladatos" scope="row"><strong>Initial number of tasks </strong></td>
      <td class="td_tabladatos_PAR" colspan="2"><input name="--itasks" type="text" id="--itasks" size="4" value="<? if(isset($_POST["--itasks"]))
				echo $_POST["--itasks"];
			else
				echo $params["--itasks"];?>"/>
        (integer) (min:<? echo $params["min_--itasks"]; ?> max:<? echo $params["max_--itasks"];?>)</td>
    </tr>
    <tr>
      <th class="th_tabladatos" scope="row"><strong>Task arrival distribution </strong></td>
      <td class="td_tabladatos_PAR" ><select name="--iatime" id="--iatime" onchange="loadProperties('params_iatime',this)"; >
	  	  <? 
		  		if(isset($_POST["--iatime"]))
					getOptions(array("constant","uniform","exponential","zipf","triangle","normal"),array("c","u","e","zipf","triangle","n"),$_POST["--iatime"]);
				
				else			
		  			getOptions(array("constant","uniform","exponential","zipf","triangle","normal"),array("c","u","e","zipf","triangle","n"),$params["--iatime"]);?>
		  
		  </select>      </td>
	   <td class="td_tabladatos_PAR" >
	   <div id="params_iatime">
	   	<?
			if(isset($_POST["--iatime"]))
				getProperties("--iatime",$_POST["--iatime"],$_POST);
			else	
				getProperties("--iatime",$params["--iatime"],$params);
		?>
	   </div>
	   </td>
    </tr>
    <tr>
      <th class="th_tabladatos" scope="row"><strong>Workload distribution </strong></td>
      <td class="td_tabladatos_PAR"><select name="--workload" id="--workload" onchange="loadProperties('params_workload',this)";>
	  		
			<? 
				if(isset($_POST["--workload"]))
					getOptions(array("constant","uniform","exponential","zipf","triangle","normal"),array("c","u","e","zipf","triangle","n"),$_POST["--workload"]);
				
				else
					getOptions(array("constant","uniform","exponential","zipf","triangle","normal"),array("c","u","e","zipf","triangle","n"),$params["--workload"]);
			?>
						</select>        </td>
	<td class="td_tabladatos_PAR"><div id="params_workload" >	<?
			if(isset($_POST["--workload"]))
				getProperties("--workload",$_POST["--workload"],$_POST);
			else
				getProperties("--workload",$params["--workload"],$params);
		?></div></td>	
    </tr>
    <tr>
      <th class="th_tabladatos" scope="row"><strong>Initial number of machines in the system</strong></td>
      <td class="td_tabladatos_PAR" colspan="2"><input name="--ihosts" type="text" id="--ihosts" size="4" value="<? if(isset($_POST["--ihosts"]))
				echo $_POST["--ihosts"];
			else	
	  			echo $params["--ihosts"];
	  ?>" />
        (integer)(min:<? echo $params["min_--ihosts"]; ?> max:<? echo $params["max_--ihosts"];?>)</td>
    </tr>
    <tr>
      <th class="th_tabladatos" scope="row"><strong>Mips distribution</strong></td>
      <td class="td_tabladatos_PAR" ><select name="--mips" id="--mips" onchange="loadProperties('params_mips',this)">

	 
	  <? 
	  	if(isset($_POST["--mips"]))
	  		getOptions(array("constant","uniform","exponential","zipf","triangle","normal"),array("c","u","e","zipf","triangle","n"),$_POST["--mips"]);
		else
			getOptions(array("constant","uniform","exponential","zipf","triangle","normal"),array("c","u","e","zipf","triangle","n"),$params["--mips"]);
		?>
					</select>      </td>
		<td class="td_tabladatos_PAR" ><div id="params_mips" >	
		<?
			if(isset($_POST["--mips"]))
				getProperties("--mips",$_POST["--mips"],$_POST);
			
			else	
				getProperties("--mips",$params["--mips"],$params);	
		?></div></td>
    </tr>
    <tr>
      <th class="th_tabladatos" scope="row"><strong>Distribution of machines joining the system  </strong></td>
      <td class="td_tabladatos_PAR"><select name="--addhost" id="--addhost" onchange="loadProperties('params_addhost',this)">
	  				<? 
						if(isset($_POST["--addhost"]))
							getOptions(array("constant","uniform","exponential","zipf","triangle","normal"),array("c","u","e","zipf","triangle","n"),$_POST["--addhost"]);
						
						else
							getOptions(array("constant","uniform","exponential","zipf","triangle","normal"),array("c","u","e","zipf","triangle","n"),$params["--addhost"]);	
					?>
						</select>        </td>
		<td class="td_tabladatos_PAR" ><div id="params_addhost" >	
		<?
			if(isset($_POST["--addhost"]))
				getProperties("--addhost",$_POST["--addhost"],$_POST);
			else
				getProperties("--addhost",$params["--addhost"],$params);
		?></div></td>
    </tr>
    <tr>
      <th class="th_tabladatos" scope="row"><strong>Distribution of machines dropping from the system </strong></th>
      <td class="td_tabladatos_PAR" ><select name="--delhost" id="--delhost" onchange="loadProperties('params_delhost',this)"/><option value="c">constant</option>
		  <? 
		  	if(isset($_POST["--delhost"]))
		  		getOptions(array("constant","uniform","exponential","zipf","triangle","normal"),array("c","u","e","zipf","triangle","n"),$_POST["--delhost"]);
		
			else
					getOptions(array("constant","uniform","exponential","zipf","triangle","normal"),array("c","u","e","zipf","triangle","n"),$params["--delhost"]);
		?>      
		  </td>
		<td class="td_tabladatos_PAR" ><div id="params_delhost" > 	
		<?
			 if(isset($_POST["--delhost"]))
				getProperties("--delhost",$_POST["--delhost"],$_POST);
			else	
				getProperties("--delhost",$params["--delhost"],$params);
		?>
		</div></td>
   </tr>
   <tr>
      <th class="th_tabladatos" scope="row"><strong>Minimum number of machines in the system </strong></th>
      <td class="td_tabladatos_PAR" colspan="2"><input name="--minhosts" type="text" id="--minhosts" size="4" value="<? 
	  	if(isset($_POST["--minhosts"]))
	  		echo $_POST["--minhosts"];
		else
			echo $params["--minhosts"];?>" />
        (integer) (min:<? echo $params["min_--minhosts"]; ?> max:<? echo $params["max_--minhosts"];?>)</td>
    </tr>
    <tr>
      <th class="th_tabladatos" scope="row"><strong>Maximum number of machines in the system </strong></th>
      <td class="td_tabladatos_PAR" colspan="2"><input name="--maxhosts" type="text" id="--maxhosts" size="4" value="<?
	  	if(isset($_POST["--maxhosts"]))
	  		echo $_POST["--maxhosts"];
		else
			echo $params["--maxhosts"];?>"/>
        (integer) (min:<? echo $params["min_--maxhosts"]; ?> max:<?  echo $params["max_--maxhosts"];?>)</td>
    </tr>
    <tr>
      <th class="th_tabladatos" scope="row"><strong>Activate reescheduling </strong></th>
      <td class="td_tabladatos_PAR" colspan="2"><select name="--reschedule" id="--reschedule" >
	  	<? 
			if(isset($_POST["--reschedule"]))
				getOptions(array("yes","no"),array("Y","N"),$_POST["--reschedule"]);
			else
				getOptions(array("yes","no"),array("Y","N"),$params["--reschedule"]);	
		?>
	  </select></td>
    </tr>
    <tr>
      <th class="th_tabladatos" scope="row"><strong>Resource selection policy </strong></th>
      <td class="td_tabladatos_PAR" colspan="2"><select name="--hostselect" id="--hostselect" >
	  	<? 
			if(isset($_POST["--hostselect"]))
				getOptions(array("idle","all"),array("idle","all"),$_POST["--hostselect"]);
			else
				getOptions(array("idle","all"),array("idle","all"),$params["--hostselect"]);	
		?>
	  </select></td>
    </tr>
    <tr>
      <th class="th_tabladatos" scope="row"><strong>Policy of scheduler activitaion </strong></th>
      <td class="td_tabladatos_PAR"  ><select name="--activate" id="--activate" onchange="loadProperties('params_activate',this)" />
	<? 
		if(isset($_POST["--activate"]))
			getOptions(array("constant","uniform","exponential","zipf","triangle","normal"),array("c","u","e","zipf","triangle","n"),$_POST["--activate"]);
		else	
			getOptions(array("constant","uniform","exponential","zipf","triangle","normal"),array("c","u","e","zipf","triangle","n"),$params["--activate"]);
		?>      
	</td>
	  <td class="td_tabladatos_PAR" ><div id="params_activate" >	
	  <? 
	  	if(isset($_POST["--activate"]))	
			getProperties("--activate",$_POST["--activate"],$_POST);
		else
			getProperties("--activate",$params["--activate"],$params);	
		?></div></td>
   	</tr >
   	<tr>
      <th class="th_tabladatos" scope="row"><strong>Local policy for managing tasks at host </strong></th>
    	 <td class="td_tabladatos_PAR" colspan="2">
    	 <select name="--allocpolicy" id="--allocpolicy">
	<? 
		if(isset($_POST["--allocpolicy"]))	
			getOptions(array("fcfs","estf"),array("fcfs","estf"),$_POST["--allocpolicy"]);
		else	
			getOptions(array("fcfs","estf"),array("fcfs","estf"),$params["--allocpolicy"]);
		?>
			
	  </select></td>
    </tr>
    <tr>
    	<th class="th_tabladatos" scope="row"><strong>Grid scheduling method</strong></th>
    	<td class="td_tabladatos_PAR" colspan="2">
    		<select name = "--strategy" id="--strategy" onchange="ActiveControl(this)">
    			<? 
    				if(isset($_POST["--strategy"]))
    				getOptions(array("GeneticAlgorithm","HierarchicGeneticStrategy", "TabuSearch", "Adhoc", "Game-based scheduler","CMA","MA","New Genetic Algorithm"),
    							array("GA_scheduler","HGS_scheduler", "TS_scheduler", 
    							"AdHoc_scheduler", "SG_scheduler","CMA_scheduler","MA_scheduler","NGA_scheduler"),
    							$_POST["--strategy"]);
    				else
    					getOptions(array("GeneticAlgorithm","HierarchicGeneticStrategy", "TabuSearch", "Adhoc", "Game-based scheduler","CMA","MA","New Genetic Algorithm"),
    								array("GA_scheduler","HGS_scheduler", "TS_scheduler", "AdHoc_scheduler", "SG_scheduler","CMA_scheduler","MA_scheduler","NGA_scheduler"),
    								$params["--strategy"]);
    			?>
    		</select>
    	</td>
    </tr>
    <tr>
    	<th class="th_tabladatos" scope="row">Adhoc heuristic</th>
    	<td class="td_tabladatos_PAR" colspan="5">
    	
    		<select name="--Adhoc_heuristic" id="--Adhoc_heuristic" style="display:none">
    		<?
    			if(isset($_POST["--Adhoc_heuristic"]))
    				getOptions(array("min_min", "max_min", "sufferage", "relative_cost", "ljfr_sjfr", "mct", "met", "sa", "kpb", "olb", "random"),
    					array("min_min", "max_min", "sufferage", "relative_cost", "ljfr_sjfr", "mct", "met", "sa", "kpb", "olb", "random"), 
    					$_POST["--Adhoc_heuristic"]);
    			else
    				getOptions(array("min_min", "max_min", "sufferage", "relative_cost", "ljfr_sjfr", "mct", "met", "sa", "kpb", "olb", "random"),
    					array("min_min", "max_min", "sufferage", "relative_cost", "ljfr_sjfr", "mct", "met", "sa", "kpb", "olb", "random"), 
    					$params["--Adhoc_heuristic"]);
	?>
    		</select>
    		<div id="divAdhoc">
    		Enabled only when Adhoc selected
    		</div>
    	</td>
    </tr>
    <tr>
    	<th class="th_tabladatos" scope="row"><strong>Scheduler activation</strong></th>
    	<td class="td_tabladatos_PAR" colspan="2">
    		<select name = "--scheduler_activation" id="--scheduler_activation">
    		<?
    			if(isset($_POST["--scheduler_activation"]))
    			getOptions(array("Static", "Dynamic"), array("time_interval(c(9999999999))", "resource_and_time_interval(c(250000))"),
    						$_POST["--scheduler_activation"]);
    						else
    						getOptions(array("Static", "Dynamic"), array("time_interval(c(9999999999))", "resource_and_time_interval(c(250000))"),
    						$params["--scheduler_activation"]);
    		 ?>
    		</select>
    	</td>
    </tr>
    <tr>
      <th class="th_tabladatos" scope="row"><strong>Independent runs </strong></th>
      <td class="td_tabladatos_PAR" colspan="2">
	  <input name="--nruns" type="text" id="--nruns" size="4" value="<? if(isset($_POST["--nruns"]))	
			echo $_POST["--nruns"];
		else
			echo $params["--nruns"];	
		?>"/>
      (integer) (min:<? echo $params["min_--nruns"]; ?> max:<? echo $params["max_--nruns"];?>)</td>
    </tr>
    <tr>
      <th class="th_tabladatos" scope="row"><strong>Seed</strong></th>
      <td class="td_tabladatos_PAR" colspan="2"><input name="--seed" type="text" id="--seed" size="4" value="<?if(isset($_POST["--seed"]))	
	  		echo $_POST["--seed"];
		else
			echo $params["--seed"];	?>" />
      (integer)</td>
    </tr>
    <tr>
      <th class="th_tabladatos" scope="row"><strong>Security</strong></th>
      <td class="td_tabladatos_PAR" colspan="3"><select name="--security" id="--security" >
      	<? if(isset($_POST["--security"]))
					getOptions(array("enable", "disable"), array("1", "0"), $_POST["--security"]);
				else
		  			getOptions(array("enable", "disable"), array("1", "0"), $params["--security"]);?>
		  			</select> </td>
    </tr>
    <tr>
      <th class="th_tabladatos" scope="row"><strong>Security parameter</strong></th>
      <td class="td_tabladatos_PAR" colspan="2">
	  <input name="--security_param" type="text" id="--security_param" size="4" value="<? if(isset($_POST["--security_param"]))	
			echo $_POST["--security_param"];
		else
			echo $params["--security_param"];	
		?>"/>
      (integer)(min:<? echo $params["min_--security_param"]; ?> max:<? echo $params["max_--security_param"];?>)</td>
    </tr>
    <tr>
      <th class="th_tabladatos" scope="row"><strong>Energy</strong></th>
      <td class="td_tabladatos_PAR" colspan="3"><select name="--energy" id="--energy" >
      	<? if(isset($_POST["--energy"]))
					getOptions(array("case I", "case II", "disable"), array("1", "2", "0"), $_POST["--energy"]);
				else
		  			getOptions(array("case I", "case II", "disable"), array("1", "2", "0"), $params["--energy"]);?>
		  			</select> </td>
    </tr>
	<tr>
      <th class="th_tabladatos" scope="row"><strong>Send by mail</strong></th>
      <td class="td_tabladatos_PAR" colspan="2"> <input name="mail" type="checkbox" id="mail" value="S" /></td>
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
