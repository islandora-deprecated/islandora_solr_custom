The islandora_solr_custom module provides a display type for the islandora_solr_search module.
It automatically looks for the fields set for display in solrconfig.xml and
outputs the fields in the order that they are defined in the requestHandler.
Islandora_solr_custom will look at the settings of islandora_solr_search to find
solrconfig.xml and the requestHandler. You can quickly customize the output 
style of your search results by changing the settings in the admin section 
or you can take full control by overriding the template file.


Reminders:
==========

* required
x useful
Key - Value - Arguments

Ability to create customized feeds. ?display=rss&feed=custom
1 default.


General values
===============
output: url of the custom feed
title
basic search url
result limit
display on each page?

Channel
=======
x * title:          value
x * link:           value
x * description:    value
x   language        value
x   copyright       value
x   managingEditor  value
x   webMaster       value
x   pubDate         value
    lastBuildDate   value
x   category        value - attributes (1) - multiple
    generator       value
    docs            value
    cloud                 - attributes (5)
    ttl             value
x   image                 - sub-elements (3)
x   textInput             - sub-elements (4)
    skipHours             - sub-elements (x)
    skipDays              - sub-elements (x)




Item
====
x * title:        value
x * link:         value
x * description:  value
x   author:       value
x   guid:         value - attributes (1)
x   enclosure:            attributes (3) - multiple
x   pubDate:      value
x   category:     value - attributes (1) - multiple
x   comments:     value
x   source:       value - attributes (1)

        $result['title']       = $doc['dc.title'];
        $result['link']        = $base_url . '/fedora/repository/' . htmlspecialchars($doc['PID'], ENT_QUOTES, 'utf-8');
        $result['description'] = $doc['dc.description'];
        $result['items'][]     = array('key' => 'author', 'value' => $doc['dc.creator']);   
        $result['items'][]     = array('key' => 'guid', 'value' => $doc['PID'], 'attributes' => array('isPermaLink' => 'false',));   // <guid isPermaLink="true">http://inessential.com/2002/09/01.php#a2</guid>
        $result['items'][]     = array('key' => 'enclosure', 'value' => '', 'attributes' => array('url' => 'http://upload.wikimedia.org/wikipedia/commons/7/7a/Basketball.png', 'length' => '1234', 'type' => 'image/png')); // enclosure 	Describes a media object that is attached to the item. More. 	<enclosure url="http://live.curry.com/mp3/celebritySCms.mp3" length="1069871" type="audio/mpeg"/>
        $result['items'][]     = array('key' => 'pubDate', 'value' => $doc['fgs.createdDate']);   
        $result['items'][]     = array('key' => 'category', 'value' => $doc['dc.subject']);
        $result['items'][]     = array('key' => 'comments', 'value' => '');
        $result['items'][]     = array('key' => 'source', 'value' => ''); 
        
        
        
        
        
        
