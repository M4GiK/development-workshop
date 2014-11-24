<?	
	include "frontend/viewManager.php";
	session_start();
	//session_register('presentation');
	
	if(!isset($_SESSION['presentation']))
		$_SESSION['presentation'] = new viewManager("ES"); 
	
?>
<?
	$action = $_GET["action"];
	$section = $_GET["section"];
	$subsection = $_GET["subsection"];
	$lang = $_GET["lang"];
	
	
	switch ($action)
	{
		case "loadcont": 
						$_SESSION['presentation']->loadContent($section,$subsection,true);
						break;
		
		case "loadsub":
						$_SESSION['presentation']->loadSubsections($section,true);
						break;			

		case "changelang":
							$_SESSION['presentation']->changeLanguage($lang);
							break;
		
		case "loadcurcont":
							$_SESSION['presentation']->loadCurrentContent(true);
							break;
		
		case "loadcursubsect":					 
							$_SESSION['presentation']->loadCurrentSubsections(true);
							break;
		case "loadsect":
						$_SESSION['presentation']->loadMenu(true);
							break;

											
	}
?>
