<?
	include "../../management/executionManager.php";
	include "../../lib/Executor.php";
	
	function generateDistibutionParam($params,$param, $value)
	{
		
		/*if($param == "--activate")
			$str.="--activate time_interval(";
		else
			$str.="$param ";	*/
		switch($value)
		{	
			case "c":
						$str .= "c(".$params[$param."_constant"].")"; 
						break;
			case "u":
						$str .= "u(".$params[$param."_low"].",".$params[$param."_high"] .")";
						break;
			case "e":
						$str .= "e(".$params[$param."_mean"].")";
						break;
			case "zipf":
						$str .= "zipf(".$params[$param."_x1"].",".$params[$param."_x2"].",".$params[$param."_p"].")";
						break;
			case "n":
						$str .= "n(".$params[$param."_mean"].",".$params[$param."_sd"].")";
						break;
			case "triangle":
						$str .= "triangle(".$params[$param."_a"].",".$params[$param."_b"].",".$params[$param."_c"].")";
						break;
		}
		
		if($param == "--activate")
			$str.=") ";
		else
			$str.=" ";	
		
		return $str;
	}
	function checkParam()
	{
	
	}
	
	function generateExecution($username,$params, $old_params, $method)
	/*
	 * 0 - Genetic Algorithm
	 * 1 - Hierarchic Genetic Strategy
	 * 2 - TabuSearch
	 * 3 - SG scheduler
	 * 4 - AdHoc
         * 5 - New Genetic Algorithm
         * 6 - CMA
         * 7 - MA
         */
	{		
		$execm = new executionManager();
		switch($method)
		{
			case 0:				
				foreach($params as $param => $value)
				{
					if($value!="" || $value=="0")
					{
						switch($param)
						{
							case "--max_time_to_spend":
							case "--opt_strategy":
								$strparams .= "$param $value ";
								$strparams = str_replace("GA_scheduler", "GA_scheduler". getGeneticAlgorithmParameters($params), $old_params);
								break;
						}				
					}
				}
				foreach($params as $param => $value)
				{
					if($value!="" || $value=="0")
					{
						switch($param)
						{
							case "--nb_evolution_step":
								$strparams .= "--ga_evolution_steps ".$value." ";
								break;
							case "--population_size":
								$strparams .= "--ga_pop_size ".$value." ";
								break;
							case "--intermediate_population_size":
								$strparams .= "--ga_intermediate_size ".$value." ";
								break;
							case "--select_choice":
								$strparams .= "--ga_select_choice ".ConvertEnumToInt($value)." ";
								break;
							case "--mutate_probability":
								$strparams .= "--ga_prob_mutate ".$value." ";
								break;
							case "--cross_probability":
								$strparams .= "--ga_prob_cross ".$value." ";
								break;
							case "--start_choice":
								$strparams .= "--ga_start_choice ".ConvertEnumToInt($value)." ";
								break;
							case "--select_extra_parameter":
								$strparams .= "--ga_select_extra ".$value." ";
								break;
							case "--crossover_operator":
								$strparams .= "--ga_cross_choice ".ConvertEnumToInt($value)." ";
								break;
							case "--select_cross_extra_parameter":
								$strparams .= "--ga_cross_extra ".$value." ";
								break;
							case "--mutate_choice":
								$strparams .= "--ga_mutate_choice ".ConvertEnumToInt($value)." ";
								break;
							case "--select_mutate_extra_parameter":
								$strparams .= "--ga_mutate_extra ".$value." ";
								break;
							case "--replace_if_better":
								if($value == true)
									$strparams .= "--ga_replace_if_better 1 ";
								else
									$strparams .= "--ga_replace_if_better 0 ";
								break;
							case "--replace_generational":
								if($value)
									$strparams .= "--ga_replace_generational 1 ";
								else
									$strparams .= "--ga_replace_generational 0 ";
								break;
						}				
					}
				}
				$strparams = $old_params;
				break;
			case 1:
		
				foreach($params as $param => $value)
				{
					//echo $param." ".$value."<br/>";
					switch($param)
					{
						case "--hgs_pop_size":
							$strparams .= "--ga_pop_size ".$value." ";
							break;
						case "--hgs_intermediate_size":
							$strparams .= "--ga_intermediate_size ".$value." ";
							break;
						case "--hgs_prob_cross":
							$strparams .= "--ga_prob_cross ".$value." ";
							break;
						case "--hgs_prob_mutate":
							$strparams .= "--ga_prob_mutate ".$value." ";
							break;
						case "--hgs2_pop_size":
							$strparams .= $param." ".$value." ";
							break;
						case "--hgs2_intermediate_size":
							$strparams .= $param." ".$value." ";
							break;
						case "--hgs2_prob_cross":
							$strparams .= $param." ".$value." ";
							break;
						case "--hgs2_prob_mutate":
							$strparams .= $param." ".$value." ";
							break;
						case "--hgs3_pop_size":
							$strparams .= $param." ".$value." ";
							break;
						case "--hgs3_intermediate_size":
							$strparams .= $param." ".$value." ";
							break;
						case "--hgs3_prob_cross":
							$strparams .= $param." ".$value." ";
							break;
						case "--hgs3_prob_mutate":
							$strparams .= $param." ".$value." ";
							break;
						case "--hgs4_pop_size":
							$strparams .= $param." ".$value." ";
							break;
						case "--hgs4_intermediate_size":
							$strparams .= $param." ".$value." ";
							break;
						case "--hgs4_prob_cross":
							$strparams .= $param." ".$value." ";
							break;
						case "--hgs4_prob_mutate":
							$strparams .= $param." ".$value." ";
							break;
						case "--hgs5_pop_size":
							$strparams .= $param." ".$value." ";
							break;
						case "--hgs5_intermediate_size":
							$strparams .= $param." ".$value." ";
							break;
						case "--hgs5_prob_cross":
							$strparams .= $param." ".$value." ";
							break;
						case "--hgs5_prob_mutate":
							$strparams .= $param." ".$value." ";
							break;
					}
				}
				$strparams .= $old_params;
				break;
			case 2:
					$strparams = $old_params;
				break;
			case 3:
				foreach($params as $param => $value)
				{
					if($value!="" || $value=="0")
					{
						switch($param)
						{
							case "--max_time_to_spend":
							case "--opt_strategy":
								$strparams .= "$param $value ";
								$strparams = str_replace("SG_scheduler", "SG_scheduler". getGeneticAlgorithmParameters($params), $old_params);
								break;
						}				
					}
				}
				foreach($params as $param => $value)
				{
					if($value!="" || $value=="0")
					{
						//echo $param." ".$value;
						switch($param)
						{
							case "--nb_evolution_step":
									$strparams .= "--ga_evolution_steps ".$value." ";
									break;
								case "--population_size":
									$strparams .= "--ga_pop_size ".$value." ";
									break;
								case "--intermediate_population_size":
									$strparams .= "--ga_intermediate_size ".$value." ";
									break;
								case "--select_choice":
									$strparams .= "--ga_select_choice ".ConvertEnumToInt($value)." ";
									break;
								case "--mutate_probability":
									$strparams .= "--ga_prob_mutate ".$value." ";
									break;
								case "--cross_probability":
									$strparams .= "--ga_prob_cross ".$value." ";
									break;
								case "--start_choice":
									$strparams .= "--ga_start_choice ".ConvertEnumToInt($value)." ";
									break;
								case "--select_extra_parameter":
									$strparams .= "--ga_select_extra ".$value." ";
									break;
								case "--crossover_operator":
									$strparams .= "--ga_cross_choice ".ConvertEnumToInt($value)." ";
									break;
								case "--select_cross_extra_parameter":
									$strparams .= "--ga_cross_extra ".$value." ";
									break;
								case "--mutate_choice":
									$strparams .= "--ga_mutate_choice ".ConvertEnumToInt($value)." ";
									break;
								case "--select_mutate_extra_parameter":
									$strparams .= "--ga_mutate_extra ".$value." ";
									break;
								case "--replace_if_better":
									if($value == true)
										$strparams .= "--ga_replace_if_better 1 ";
									else
										$strparams .= "--ga_replace_if_better 0 ";
									break;
								case "--replace_generational":
									if($value)
										$strparams .= "--ga_replace_generational 1 ";
									else
										$strparams .= "--ga_replace_generational 0 ";
									break;
						}				
					}
				}
				break;
			case 4:
				$strparams = $old_params;
				break;
                        case 5:
				foreach($params as $param => $value)
				{
					if($value!="" || $value=="0")
					{
						switch($param)
						{
							case "--max_time_to_spend":
							case "--opt_strategy":
								$strparams .= "$param $value ";
								$strparams = str_replace("NGA_scheduler", "NGA_scheduler". getGeneticAlgorithmParameters($params), $old_params);
								break;
						}
					}
				}
				foreach($params as $param => $value)
				{
					if($value!="" || $value=="0")
					{
						switch($param)
						{
							case "--nb_evolution_step":
								$strparams .= "--ga_evolution_steps ".$value." ";
								break;
							case "--population_size":
								$strparams .= "--ga_pop_size ".$value." ";
								break;
							case "--intermediate_population_size":
								$strparams .= "--ga_intermediate_size ".$value." ";
								break;
							case "--select_choice":
								$strparams .= "--ga_select_choice ".ConvertEnumToInt($value)." ";
								break;
							case "--mutate_probability":
								$strparams .= "--ga_prob_mutate ".$value." ";
								break;
							case "--cross_probability":
								$strparams .= "--ga_prob_cross ".$value." ";
								break;
							case "--start_choice":
								$strparams .= "--ga_start_choice ".ConvertEnumToInt($value)." ";
								break;
							case "--select_extra_parameter":
								$strparams .= "--ga_select_extra ".$value." ";
								break;
							case "--crossover_operator":
								$strparams .= "--ga_cross_choice ".ConvertEnumToInt($value)." ";
								break;
							case "--select_cross_extra_parameter":
								$strparams .= "--ga_cross_extra ".$value." ";
								break;
							case "--mutate_choice":
								$strparams .= "--ga_mutate_choice ".ConvertEnumToInt($value)." ";
								break;
							case "--select_mutate_extra_parameter":
								$strparams .= "--ga_mutate_extra ".$value." ";
								break;
							case "--replace_if_better":
								if($value == true)
									$strparams .= "--ga_replace_if_better 1 ";
								else
									$strparams .= "--ga_replace_if_better 0 ";
								break;
							case "--replace_generational":
								if($value)
									$strparams .= "--ga_replace_generational 1 ";
								else
									$strparams .= "--ga_replace_generational 0 ";
								break;
						}
					}
				}
				$strparams = $old_params;
				break;
                        case 6:
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
				$strparams = $old_params;
				break;
                        case 7:
				foreach($_POST as $param => $value)
                                {
                                        if($value!="" || $value=="0")
                                        {
                                                switch($param)
                                                {
                                                        case "--max_time_to_spend":
                                                        case "--opt_strategy":
                                                                $strparams .= "$param $value ";
                                                                $strparams = str_replace("MA_scheduler", "MA_scheduler". getGeneticAlgorithmParameters($_POST), $old_params);
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
                                                        case "--ma_nb_generations":
                                                                $strparams .= "--ma_nb_generations ".$value." ";
                                                                break;
                                                        case "--ma_population_size":
                                                                $strparams .= "--ma_population_size ".$value." ";
                                                                break;
                                                        case "--ma_nb_solutions_to_recombine":
                                                                $strparams .= "--ma_nb_solutions_to_recombine ".$value." ";
                                                                break;
                                                        case "--ma_nb_recombinations":
                                                                $strparams .= "--ma_nb_recombinations ".$value." ";
                                                                break;
                                                        case "--ma_nb_mutations":
                                                                $strparams .= "--ma_nb_mutations ".$value." ";
                                                                break;
                                                        case "--ma_start_choice":
                                                                $strparams .= "--ma_start_choice ".ConvertEnumToInt($value)." ";
                                                                break;
                                                        case "--ma_select_choice":
                                                                $strparams .= "--ma_select_choice ".ConvertEnumToInt($value)." ";
                                                                break;
                                                        case "--ma_select_extra_parameter":
                                                                $strparams .= "--ma_select_extra_parameter ".$value." ";
                                                                break;
                                                        case "--ma_cross_choice":
                                                                $strparams .= "--ma_cross_choice ".ConvertEnumToInt($value)." ";
                                                                break;
                                                        case "--ma_recombine_selection":
                                                                $strparams .= "--ma_recombine_selection ".ConvertEnumToInt($value)." ";
                                                                break;
                                                        case "--ma_rec_selection_extra":
                                                                $strparams .= "--ma_rec_selection_extra ".$value." ";
                                                                break;
                                                        case "--ma_mutate_choice":
                                                                $strparams .= "--ma_mutate_choice ".ConvertEnumToInt($value)." ";
                                                                break;
                                                        case "--ma_mutate_extra_parameter":
                                                                $strparams .= "--ma_mutate_extra_parameter ".$value." ";
                                                                break;
                                                        case "--ma_mutate_selection":
                                                                $strparams .= "--ma_mutate_selection ".ConvertEnumToInt($value)." ";
                                                                break;
                                                        case "--ma_mut_selection_extra":
                                                                $strparams .= "--ma_mut_selection_extra ".$value." ";
                                                                break;
                                                        case "--ma_nb_ls_not_improving_iterations":
                                                                $strparams .= "--ma_nb_ls_not_improving_iterations ".$value." ";
                                                                break;
                                                        case "--ma_add_only_if_better":
                                                                if($value)
                                                                        $strparams .= "--ma_add_only_if_better 1 ";
                                                                else
                                                                        $strparams .= "--ma_add_only_if_better 0 ";
                                                                break;
                                                }
                                        }
                                }
				$strparams = $old_params;
				break;
			default:
				break;
		}
		//echo $strparams;
		//echo "<br/><br/><br/>";
		//setcookie('strparams', $strparams, time() +3600 );
		//header("location:wait.php");
		$execm->newExecution($username, $strparams, $status_mail);
		//return $exe;
	}
	function ConvertEnumToInt($param)
	{
		switch ($param)
		{
			case "SelectRandom":
			case "CrossOnePoint":
			case "MutateMove":
			case "StartLJFRSJFR":
			case "RecombineOnePoint":
			case "LocalMove":
			case "Panmictic":
			case "FixedLineSweep":
				return 0;
				break;
			case "SelectBest":
			case "CrossTwoPoints":
			case "MutateSwap":
			case "StartMCT":
			case "RecombineUniform":
			case "SteepestLocalMove":
			case "L5":
			case "FixedRandomSweep":
				return 1;
				break;
			case "SelectLinearRanking":
			case "CrossUniform":
			case "MutateBoth":
			case "StartRandom":
			case "RecombineFitnessBased":
			case "LocalSwap":
			case "L9":
			case "NewRandomSweep":
				return 2;
				break;
			case "SelectExpRanking":
			case "CrossFitnessBased":
			case "MutateRebalancing":
			case "StartAll":
			case "SteepestLocalSwap":
			case "C9":
				return 3;
				break;
			case "SelectBinaryTournament":
			case "CrossPMX":
			case "StartMinMin":
			case "LocalRebalance":
			case "C13":
				return 4;
				break; 
			case "SelectNTournament":
			case "CrossOX":
			case "StartRelativeCost":
			case "DeepLocalRebalance":
				return 5;
				break;
			case "CrossCX":
			case "StartKPB":
			case "LocalMCTMove":
				return 6;
				break;
			case "StartAll2":
			case "LocalMCTSwap":
				return 7;
				break;
			case "LocalMFTMove":
				return 8;
				break;
			case "LocalMFTSwap":
				return 9;
				break;
			case "LocalFlowtimeRebalance":
				return 10;
				break;
			case "EmptiestRessourceRebalance":
				return 11;
				break;
			case "EmptiestRessourceFlRebalance":
				return 12;
				break;
			case "LocalShortHop":
				return 13;
				break;
			case "VNS":
				return 14;
				break;
			case "LocalTabuHop":
				return 15;
				break;
			case "Run":
				break;
			default:
				//$result .= $value;
					//$result .= ","; 
					break;
		}
	}
	
	function getGeneticAlgorithmParameters($params)
	{
		$result = "(";
		//foreach ($params as $param => $value)
		//{echo "<br/>".$param."<br/>";}
		foreach ($params as $param => $value)
		{
			switch($param)
			{ 			
				case "--opt_strategy":
					if($value == true)
					{
						$result .= "h";
					}
					else
					{
						$result .= "s";
					}
					$result .= ",";
					break;
				case "--max_time_to_spend":
					$result .= $value;
					$result .= ",";
					break;
			}
		}
		$result = substr($result, 0, strlen($result)-1);
		$result .= ")";
		return $result;
	}
?>