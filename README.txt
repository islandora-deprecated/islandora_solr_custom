The islandora_solr_custom module provides a display type for the islandora_solr_search module.
Islandora_solr_custom automatically looks for the fields set for display in solrconfig.xml.
It outputs the fields in the order that they are defined in the requestHandler.
Islandora_solr_custom will look at the settings of islandora_solr_search to find
solrconfig.xml and the requestHandler. You can quickly customize the output 
style of your search results by changing the settings in the admin section 
or you can take full control by overriding the template file.

