# PGameDB
PGameDatabase is an open-source stand-alone tool for tracking, organizing, and managing game collections and related media.  
<br><b>Features:<b><br>
<ul>
	<li>Automated collection of game information, box shots, and screen shots from the internet via scraper plugins (Mobygames plugin is included)</li>
	<li>Tracking of game related media such as manuals, hint books, and audio files</li>
	<li>Text search of any field of the database</li>
	<li>Game lists are filterable by:</li>
    <ul>
		<li>Genre</li>
		<li>Developer</li>
		<li>Platform or Platform Type (e.g. Handheld, PC, Console)</li>
		<li>Release Year</li>
		<li>Played / Unplayed</li>
		<li>Series</li>
		<li>Publisher</li>
		<li>Location (e.g. CD Case 1)</li>
		<li>Custom User-Defined Categories (e.g. Cyberpunk, Sci-fi, Backlog, etc.)</li>
		<li>Rating</li>
    </ul>
	<li>Custom views, allowing user defined category listing</li>
</ul>
<br><br>
     <img src="https://github.com/centicosm/PGameDB/blob/master/Wiki/ScreenShot1.jpg" width="541px" height="463px" align="center" />
<br>
     <img src="https://github.com/centicosm/PGameDB/blob/master/Wiki/ScreenShot2.jpg" width="541px" height="463px" align="center" />
<br><br>
<b>Use</b>
<br>
-Start a new database or open a pre-existing database via the file menu.<br><br>
-Toolbar icons have descriptions available with mouse over<br><br>
-Add a game to the database using the selected scraper or manually via the tool bar icons.<br><br>
-Double click a game name from the game list to edit the details<br><br>
-Update the currently selected game using the scraper via the toolbar icon.<br><br>
-View mode controls the currently displayed game list (which can be further filtered using the filter toolbar).<br><br>
-Unsorted is the list of all game entries, category makes the list of games the last selected category (e.g. Genre:Horror or Developer:Origin).  
-Custom view mode is a game list consisting of any number of user selected categories.<br><br>
-Filter the current game list using the filter toolbar.<br><br>
-You can switch to a category view by clicking on any field in the game detail pane (i.e. clicking on action in the genre field in a game description with switch the view mode to category mode and the game list would show all games in the action genre).<br><br>
-You can use the folder icons on the toolbar to add categories to the custom view mode.  'Add category to custom' icon will add the currently selected category to the custom view mode.  'Add game category' icon will allow you to enter the category manually (see below for formatting details).<br><br>
-The custom view mode can also by edited via the edit menu.  New categories for the custom can be added by entering either a user defined category name (e.g. Backlog games) or a field:value pair (e.g. Year:1995).<br><br>
-The preferences option under edit will allow you to change settings related to media paths and scraping.<br>
<br><br>
<b>Building:</b><br>
Using Visual Studio, simply open the solution file and build the solution.  The executable will be generated in the Output directory which comes preconfigured to match the default path settings. 
