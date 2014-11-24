<?
	class Executor
	{
		var $params;
		var $output;
		var $trace;
 	
		function Executor($params="", $output="", $trace="")
		{
			$this->params = $params;
			$this->output = $output;
			$this->trace = $trace;
		}
		
		function execute()
		{
			$cm  =  new configManager();
			
			$via = $cm->getExecutionType();
			if($via == "condor")
			{
				$this->params.= " -t ".$this->trace;
				$id = $this->condor_submit($this->params,$this->output);
			}
			
			else
			{	
			//$this->params."--ttasks 64 --iatime e(50) --workload e(100000) --ihosts 32 --mips e(500) --addhost e(10000) --delhost e(12000) --maxhosts 2147483647 --hostselect all --activate time_interval(c(300)) --allocpolicy fcfs --nruns 1 --seed 123467
				$this->params.= " -t ".OUTPATH."/".$this->trace;
				
				$parsed_params =str_replace(array("(",")"), array("\(","\)") , $this->params);
				$bin = SIMPATH." ".$parsed_params;
			
				exec($bin,$cq_out,$return);
				$file = OUTPATH."/".$this->output.".txt";
				$fd = fopen($file,"w");
				for($i = 0; $i < count($cq_out);$i++)
				{
					$line = $cq_out[$i];
					fputs($fd, "$line\n", 4096);
				}
				fclose($fd);
				$id = 0;
			}
			$result["condor_id"] = $id;
			$result["tipo"] = $via;	
			
			return $result;	
		}
		
		function cancel($id)
		{
		}
		
		function getCondorId($out)
		{
		
			list($num_jobs, $cluster_id, $proc_id) = sscanf($out[2] ,"%d job(s) submitted to cluster %d.%d");
				if (empty($proc_id))
					$proc_id = 0;
		
		
			$condor_id = $cluster_id.".".$proc_id;
		
			return $condor_id;
		}
	
		function condor_q($condor_id)
		{
			exec(CONDORPATH."/condor_q $condor_id", $cq_out, $cq_ret);
			
			$jobline = array_pop($cq_out);
			$jobline = ereg_replace(" +", " ", $jobline);
			$out = explode(" ", $jobline);
			
			$aux = $out[1];
			
			if($out[1] == $condor_id)
				return false;
				
			else 
				return true;
			
		}
		function condor_submit($params,$output)
		{
			$descriptorspec = array(
			   0 => array("pipe", "r"),  // stdin is a pipe that the child will read from
			   1 => array("pipe", "w"),  // stdout is a pipe that the child will write to
			   2 => array("file", "/tmp/error-output.txt", "a") // stderr is a file to write to
			);
			
			
			$process = proc_open(CONDORPATH."/condor_submit", $descriptorspec, $pipes);
			
			if (is_resource($process)) 
			{
				// $pipes now looks like this:
				// 0 => writeable handle connected to child stdin
				// 1 => readable handle connected to child stdout
				// Any error output will be appended to /tmp/error-output.txt
				$description = "initialdir = ".OUTPATH."\n";
				$description .= "Executable = ".SIMPATH."\n";
				$description .= "arguments = $params\n";
				$description .= "output = $output.txt\n";
				$description .= "error = error.txt\n";
				$description .= "log = log.txt\n";
				$description .= "hold = false\n";
				$description .= "should_transfer_files = YES\n";
				$description .= "when_to_transfer_output = ON_EXIT\n";
				$description .= "queue\n";
				
				//echo nl2br($description);
				fputs($pipes[0], $description);
				fflush($pipes[0]);
			
				fclose($pipes[0]);
			
				sleep(3);
				$i = 0;
				while (! feof($pipes[1]))
				{
					$retval[$i] = fgets($pipes[1]);
					$i++;
				}
				
				//print_r($retval);
				$condor_id = $this->getCondorId($retval);
				//echo "<br>condor_id:$condor_id<br>";
				
				fclose($pipes[1]);
			
				// It is important that you close any pipes before calling
				// proc_close in order to avoid a deadlock
				$return_value = proc_close($process);
			
				//echo "command returned $return_value<br>";
				
				
				
				
			}
			
			return $condor_id;
		}
}	
	
?>
