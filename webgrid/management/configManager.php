<?
	/*
 	* @class contentManager
 	* @brief Manager de los contenidos
	* @autor David Martos Expósito 
	*
	*/
	
	class configManager
	{
		var $linkdb;
		var $db;
		var $userdb;
		var $passworddb;
 	
		function configManager()
		{
			$this->userdb = USERDB;
		$this->passworddb = PASSWORDDB;
		$this->db = DB;
		$this->linkdb = mysql_connect (MYSQLMACHINE, $this->userdb, $this->passworddb) 
		or die ('I cannot connect to the database because: ' . mysql_error());
		
		mysql_select_db ($this->db, $this->linkdb) or die ('No puedo seleccionar la bd'.mysql_error());
		
		}
		
		function editDefaultParams($params)
		{
			$str_params = $this->ArrayParamsToString($params);
			$sql = "UPDATE config SET valor='$str_params' WHERE parametro='default_params'";
			
			if($result = mysql_query($sql, $this->linkdb))
				return true;
			
			else
				return false;	
		}
		
		function editNewDefaultParams($params)
		{
			//$str_params = $this->ArrayParamsToString($params);
			$default_params = "";
			$default_params_GA = "";
			$default_params_HGS = "";
			$default_params_HGSBranches = "";
			$default_params_TS = "";
			$default_params_SG = "";
			
			foreach($params as $param => $value)
			{
				//echo $param." ".$value."<br/>";
				switch($param)
				{
					case "--ttasks":
					case "max_--ttasks":
					case "min_--ttasks":
					case "--itasks":
					case "max_--itasks":
					case "min_--itasks":
					case "--iatime":
					case "--iatime_mean":
					case "--iatime_constant":
					case "--iatime_low":
					case "--iatime_high":
					case "--iatime_sd":
					case "--iatime_x1":
					case "--iatime_x2":
					case "--iatime_p":
					case "--iatime_a":
					case "--iatime_b":
					case "--iatime_c":
					case "--workload":
					case "--workload_mean":
					case "--workload_constant":
					case "--workload_low":
					case "--workload_high":
					case "--workload_sd":
					case "--workload_x1":
					case "--workload_x2":
					case "--workload_p":
					case "--workload_a":
					case "--workload_b":
					case "--workload_c":
					case "--ihosts":
					case "max_--ihosts":
					case "min_--ihosts":
					case "--mips":
					case "--mips_mean":
					case "--mips_constant":
					case "--mips_low":
					case "--mips_high":
					case "--mips_sd":
					case "--mips_x1":
					case "--mips_x2":
					case "--mips_p":
					case "--mips_a":
					case "--mips_b":
					case "--mips_c":
					case "--addhost":
					case "--addhost_mean":
					case "--addhost_constant":
					case "--addhost_low":
					case "--addhost_high":
					case "--addhost_sd":
					case "--addhost_x1":
					case "--addhost_x2":
					case "--addhost_p":
					case "--addhost_a":
					case "--addhost_b":
					case "--addhost_c":
					case "--delhost":
					case "--delhost_mean":
					case "--delhost_constant":
					case "--delhost_low":
					case "--delhost_high":
					case "--delhost_sd":
					case "--delhost_x1":
					case "--delhost_x2":
					case "--delhost_p":
					case "--delhost_a":
					case "--delhost_b":
					case "--delhost_c":
					case "--minhosts":
					case "max_--minhosts":
					case "min_--minhosts":
					case "--maxhosts":
					case "max_--maxhosts":
					case "min_--maxhosts":
					case "--reschedule":
					case "--hostselect":
					case "--activate":
					case "--activate_constant":
					case "--activate_constant":
					case "--activate_low":
					case "--activate_high":
					case "--activate_sd":
					case "--activate_x1":
					case "--activate_x2":
					case "--activate_p":
					case "--activate_a":
					case "--activate_b":
					case "--activate_c":
					case "--allocpolicy":
					case "--nruns":
					case "max_--nruns":
					case "min_--nruns":
					case "--seed":
						$default_params .= $param." ".$value." ";
						break;
					case "--GA_pop_size":
						$default_params_GA .= "--population_size ".$value." ";
						break;
					case "--GA_ipop_size":
						$default_params_GA .= "--intermediate_population_size ".$value." ";
						break;
					case "--GA_max_time_to_spend":
						$default_params_GA .= "--max_time_to_spend ".$value." ";
						break;
					case "--GA_MIN_max_time_to_spend":
						$default_params_GA .= "min_--max_time_to_spend ".$value." ";
						break;
					case "--GA_MAX_max_time_to_spend":
						$default_params_GA .= "max_--max_time_to_spend ".$value." ";
						break;
					case "--GA_evolution_steps":
						$default_params_GA .= "--nb_evolution_step ".$value." ";
						break;
					case "--GA_MIN_evolution_steps":
						$default_params_GA .= "min_--nb_evolution_step ".$value." ";
						break;
					case "--GA_MAX_evolution_steps":
						$default_params_GA .= "max_--nb_evolution_step ".$value." ";
						break;

					case "--GA_extra_parameter":
						$default_params_GA .= "--select_extra_parameter ".$value." ";
						break;
					case "--GA_MAX_extra_parameter":
						$default_params_GA .= "max_--select_extra_parameter ".$value." ";
						break;
					case "--GA_MIN_extra_parameter":
						$default_params_GA .= "min_--select_extra_parameter ".$value." ";
						break;
					case "--GA_cross_probability":
						$default_params_GA .= "--cross_probability ".$value." ";
						break;
					case "--GA_MIN_cross_probability":
						$default_params_GA .= "min_--cross_probability ".$value." ";
						break;
					case "--GA_MAX_cross_probability":
						$default_params_GA .= "max_--cross_probability ".$value." ";
						break;
					case "--GA_cross_extra_parameter":
						$default_params_GA .= "--select_cross_extra_parameter ".$value." ";
						break;
					case "--GA_MAX_cross_extra_parameter":
						$default_params_GA .= "max_--select_cross_extra_parameter ".$value." ";
						break;
					case "--GA_MIN_cross_extra_parameter":
						$default_params_GA .= "min_--select_cross_extra_parameter ".$value." ";
						break;
					case "--GA_mutate_probability":
						$default_params_GA .= "--mutate_probability ".$value." ";
						break;
					case "--GA_MIN_mutate_probability":
						$default_params_GA .= "min_--mutate_probability ".$value." ";
						break;
					case "--GA_MAX_mutate_probability":
						$default_params_GA .= "max_--mutate_probability ".$value." ";
						break;
					case "--GA_mutate_extra_parameter":
						$default_params_GA .= "--select_mutate_extra_parameter ".$value." ";
						break;	
					case "--GA_MAX_mutate_extra_parameter":
						$default_params_GA .= "max_--select_mutate_extra_parameter ".$value." ";
						break;
					case "--GA_MIN_mutate_extra_parameter":
						$default_params_GA .= "min_--select_mutate_extra_parameter ".$value." ";
						break;
						
						
					case "--HGS_metaepoch_length":
						$default_params_HGS .= "--evolution_steps ".$value." ";
						break;
					case "--HGS_MIN_metaepoch_length":
						$default_params_HGS .= "min_--evolution_steps ".$value." ";
						break;
					case "--HGS_MAX_metaepoch_length":
						$default_params_HGS .= "max_--evolution_steps ".$value." ";
						break;
					case "--HGS_nb_of_metaepoch":
						$default_params_HGS .= "--ga_number_metaepochs ".$value." ";
						break;
					case "--HGS_MAX_nb_of_metaepoch":
						$default_params_HGS .= "max_--ga_number_metaepochs ".$value." ";
						break;
					case "--HGS_MIN_nb_of_metaepoch":
						$default_params_HGS .= "min_--ga_number_metaepochs ".$value." ";
						break;
					case "--HGS_max_time_to_spend":
						$default_params_HGS .= "--max_time_to_spend ".$value." ";
						break;
					case "--HGS_MIN_max_time_to_spend":
						$default_params_HGS .= "min_--max_time_to_spend ".$value." ";
						break;
					case "--HGS_MAX_max_time_to_spend":
						$default_params_HGS .= "max_--max_time_to_spend ".$value." ";
						break;
					case "--HGS_extra_parameter":
						$default_params_HGS .= "--select_extra_parameter ".$value." ";
						break;
					case "--HGS_MIN_extra_parameter":
						$default_params_HGS .= "min_--select_extra_parameter ".$value." ";
						break;
					case "--HGS_MAX_extra_parameter":
						$default_params_HGS .= "max_--select_extra_parameter ".$value." ";
						break;
					case "--HGS_cross_extra_param":
						$default_params_HGS .= "--cross_extra_parameter ".$value." ";
						break;
					case "--HGS_MIN_cross_extra_parameter":
						$default_params_HGS .= "min_--cross_extra_parameter ".$value." ";
						break;
					case "--HGS_MAX_cross_extra_parameter":
						$default_params_HGS .= "max_--cross_extra_parameter ".$value." ";
						break;
					case "--HGS_mutate_extra_param":
						$default_params_HGS .= "--mutate_extra_parameter ".$value." ";
						break;
					case "--HGS_MIN_mutate_extra_parameter":
						$default_params_HGS .= "min_--mutate_extra_parameter ".$value." ";
						break;
					case "--HGS_MAX_mutate_extra_parameter":
						$default_params_HGS .= "max_--mutate_extra_parameter ".$value." ";
						break;
					case "--HGS_env_length":
						$default_params_HGS .= "--enviroment_length ".$value." ";
						break;
					case "--HGS_MIN_env_length":
						$default_params_HGS .= "min_--enviroment_length ".$value." ";
						break;
					case "--HGS_MAX_env_length":
						$default_params_HGS .= "max_--enviroment_length ".$value." ";
						break;
						
					case "--HGS2_pop_size":
						$default_params_HGSBranches .= "--hgs_pop_size ".$value." ";
						break;
					case "--HGS2_ipop_size":
						$default_params_HGSBranches .= "--hgs_intermediate_size ".$value." ";
						break;
					case "--HGS2_cross_probab":
						$default_params_HGSBranches .= "--hgs_prob_cross ".$value." ";
						break;
					case "--HGS2_MIN_cross_probab":
						$default_params_HGSBranches .= "min_--hgs_prob_cross ".$value." ";
						break;
					case "--HGS2_MAX_cross_probab":
						$default_params_HGSBranches .= "max_--hgs_prob_cross ".$value." ";
						break;
					case "--HGS2_mutate_probab":
						$default_params_HGSBranches .= "--hgs_prob_mutate ".$value." ";
						break;
					case "--HGS2_MIN_mutate_probab":
						$default_params_HGSBranches .= "min_--hgs_prob_mutate ".$value." ";
						break;
					case "--HGS2_MAX_mutate_probab":
						$default_params_HGSBranches .= "max_--hgs_prob_mutate ".$value." ";
						break;
						
					case "--TS_nb_iterations":
						$default_params_TS .= "--tb_nb_iterations ".$value." ";
						break;
					case "--TS_MIN_nb_iterations":
						$default_params_TS .= "min_--tb_nb_iterations ".$value." ";
						break;
					case "--TS_MAX_nb_iterations":
						$default_params_TS .= "max_--tb_nb_iterations ".$value." ";
						break;
					case "--TS_tabu_size":
						$default_params_TS .= "--tb_tabu_size ".$value." ";
						break;
					case "--TS_MIN_tabu_size":
						$default_params_TS .= "min_--tb_tabu_size ".$value." ";
						break;
					case "--TS_MAX_tabu_size":
						$default_params_TS .= "max_--tb_tabu_size ".$value." ";
						break;
					case "--TS_MIN_max_tabu_status":
						$default_params_TS .= "min_--tb_max_tabu_status ".$value." ";
						break;
					case "--TS_MAX_max_tabu_status":
						$default_params_TS .= "max_--tb_max_tabu_status ".$value." ";
						break;
					case "--TS_min_tabu_status":
						$default_params_TS .= "--tb_min_tabu_status ".$value." ";
						break;
					case "--TS_max_repetitions":
						$default_params_TS .= "--tb_max_repetitions ".$value." ";
						break; 
					case "--TS_MIN_max_repetitions":
						$default_params_TS .= "min_--tb_max_repetitions ".$value." ";
						break; 
					case "--TS_MAX_max_repetitions":
						$default_params_TS .= "max_--tb_max_repetitions ".$value." ";
						break; 
					case "--TS_nb_intensifications":
						$default_params_TS .= "--tb_nb_intensifications ".$value." ";
						break;
					case "--TS_MIN_nb_intensifications":
						$default_params_TS .= "min_--tb_nb_intensifications ".$value." ";
						break;
					case "--TS_MAX_nb_intensifications":
						$default_params_TS .= "max_--tb_nb_intensifications ".$value." ";
						break;
					case "--TS_nb_diversifications":
						$default_params_TS .= "--tb_nb_diversifications ".$value." ";
						break;
					case "--TS_MIN_nb_diversifications":
						$default_params_TS .= "min_--tb_nb_diversifications ".$value." ";
						break;
					case "--TS_MAX_nb_diversifications":
						$default_params_TS .= "max_--tb_nb_diversifications ".$value." ";
						break;
					case "--TS_elite_size":
						$default_params_TS .= "--tb_elite_size ".$value." ";
						break;
					case "--TS_MIN_elite_size":
						$default_params_TS .= "min_--tb_elite_size ".$value." ";
						break;
					case "--TS_MAX_elite_size":
						$default_params_TS .= "max_--tb_elite_size ".$value." ";
						break;
					case "--TS_aspiration_value":
						$default_params_TS .= "--tb_aspiration_value ".$value." ";
						break;
					case "--TS_MIN_aspiration_value":
						$default_params_TS .= "min_--tb_aspiration_value ".$value." ";
						break;
					case "--TS_MAX_aspiration_value":
						$default_params_TS .= "max_--tb_aspiration_value ".$value." ";
						break;
					case "--TS_max_time_to_spend":
						$default_params_TS .= "--max_time_to_spend ".$value." ";
						break;
					case "--TS_MIN_max_time_to_spend":
						$default_params_TS .= "max_--max_time_to_spend ".$value." ";
						break;
					case "--TS_MAX_max_time_to_spend":
						$default_params_TS .= "min_--max_time_to_spend ".$value." ";
						break;
						
					case "--SG_nb_players":
						$default_params_SG .= "--number_players ".$value;
						break;
						
					default:
						break;
				}
			}
			
			$sql1 = "UPDATE config SET valor='$default_params' WHERE parametro='default_params'";
			$sql2 = "UPDATE config SET valor='$default_params_GA' WHERE parametro='default_params_GA'";
			$sql3 = "UPDATE config SET valor='$default_params_HGS' WHERE parametro='default_params_HGS'";
			$sql4 = "UPDATE config SET valor='$default_params_HGSBranches' WHERE parametro='default_params_HGSBranches'";
			$sql5 = "UPDATE config SET valor='$default_params_TS' WHERE parametro='default_params_TS'";
			$sql6 = "UPDATE config SET valor='$default_params_SG' WHERE parametro='default_params_SG'";
			
			if($result1 = mysql_query($sql1, $this->linkdb) && $result2 = mysql_query($sql2, $this->linkdb) &&
				$result3 = mysql_query($sql3, $this->linkdb) && $result4 = mysql_query($sql4, $this->linkdb) &&
				$result5 = mysql_query($sql5, $this->linkdb))
				return true;
			
			else
				return false;	
			
		}
		
		function editMaxParams($params)
		{
			$str_params = $this->ArrayParamsToString($params);
			$sql = "UPDATE config SET valor='$str_params' WHERE parametro='max_params'";
			
			if($result = mysql_query($sql, $this->linkdb))
				return true;
			
			else
				return false;	
		}
		
		function getDefaultParams()
		{
			$sql = "SELECT valor FROM config WHERE parametro='default_params'"; 
			
			if($result = mysql_query($sql, $this->linkdb))
			{
				$row = mysql_fetch_array($result);
				$str_default = $row["valor"];
				mysql_free_result($result);	
			}
			
			return $this->buildArrayParams($str_default);

		}
		
		function getDefaultParamsWithParametro($parametro)
		{
			$sql = "SELECT valor FROM config WHERE parametro='$parametro'";
			
			if($result = mysql_query($sql, $this->linkdb))
			{
				$row = mysql_fetch_array($result);
				$str_default = $row["valor"];
				mysql_free_result($result);
			}
			
			return $this->buildArrayParams($str_default);
		}
		
		function getMaxExecutions()
		{
			$sql = "SELECT valor FROM config WHERE parametro='max_executions'"; 
			
			if($result = mysql_query($sql, $this->linkdb))
			{
				$row = mysql_fetch_array($result);
				$max = $row["valor"];
				mysql_free_result($result);	
			}
			
			return $max;
		}
		
		function editMaxExecutions($param)
		{
			
			$sql = "UPDATE config SET valor='$param' WHERE parametro='max_executions'";
			
			if($result = mysql_query($sql, $this->linkdb))
				return true;
			
			else
				return false;	
		}

		
		
		
		function getMail()
		{
			$sql = "SELECT mail FROM usuarios WHERE tipo='admin'"; 
			
			if($result = mysql_query($sql, $this->linkdb))
			{
				$row = mysql_fetch_array($result);
				$mail = $row["mail"];
				mysql_free_result($result);	
			}
			
			return $mail;
		}
		
		function editMail($mail)
		{
			$sql = "UPDATE usuarios SET mail='$mail' WHERE tipo='admin'";
			
			if($result = mysql_query($sql, $this->linkdb))
				return true;
			
			else
				return false;	
		}
		
		function getMessageAdmin()
		{
			$sql = "SELECT valor FROM config WHERE parametro='mensaje_admin'"; 
			
			if($result = mysql_query($sql, $this->linkdb))
			{
				$row = mysql_fetch_array($result);
				$msg = $row["valor"];
				mysql_free_result($result);	
			}
			
			return $msg;
		}
		
		function editMessageAdmin($message)
		{
			$sql = "UPDATE config SET valor='$message' WHERE parametro='mensaje_admin'";
			
			if($result = mysql_query($sql, $this->linkdb))
				return true;
			
			else
				return false;	
		}
		function editPassword($password)
		{
			$sql = "UPDATE usuarios SET password='$password' WHERE tipo='admin'";
			
			if($result = mysql_query($sql, $this->linkdb))
				return true;
			
			else
				return false;	
		}
		
		function getPassword()
		{
			$sql = "SELECT password FROM usuarios WHERE tipo='admin'"; 
			
			if($result = mysql_query($sql, $this->linkdb))
			{
				$row = mysql_fetch_array($result);
				$password = $row["password"];
				mysql_free_result($result);	
			}
			
			return $password;
		}
		
		function getExecutionType()
		{
			$sql = "SELECT valor FROM config WHERE parametro='execution_type'"; 
		
			
			if($result = mysql_query($sql, $this->linkdb))
			{
				$row = mysql_fetch_array($result);
				$type = $row["valor"];
				mysql_free_result($result);	
			}
			
			return $type;
		}
		
		function editExecutionType($param)
		{
			
			$sql = "UPDATE config SET valor='$param' WHERE parametro='execution_type'";
			
			if($result = mysql_query($sql, $this->linkdb))
				return true;
			
			else
				return false;	
		}
		
		
		function getAvisos()
		{
			$sql = "SELECT valor FROM config WHERE parametro='avisos'"; 
			
			if($result = mysql_query($sql, $this->linkdb))
			{
				$row = mysql_fetch_array($result);
				$str_default = $row["valor"];
				mysql_free_result($result);	
			}
			
			return $this->buildArrayParams($str_default);
		}
		
		function editAvisos($params)
		{
			$str_params = $this->ArrayParamsToString($params);
			$sql = "UPDATE config SET valor='$str_params' WHERE parametro='avisos'";
			
			if($result = mysql_query($sql, $this->linkdb))
				return true;
			
			else
				return false;	
		}
		
		
		
		
		function ArrayParamsToString($array)
		{
			$n = false;
			foreach($array as $key=>$value)
			{
				if($n == true)
					$str .= " $key $value";
				else
				{
					$str .= "$key $value";
					$n = true;
				}	
			}
			return $str;
		}
		
		function buildArrayParams($string)
		{
			$array = explode(" ",$string);
			$result = array();
			$i = 0;
			while($i<count($array))
			{
				$result[$array[$i]] = $array[$i+1];
				$i+=2;
			}
			return $result;
		}
		
	
		
		
		
		
	}

?>