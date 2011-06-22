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
o not going to use
F Fixed value


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
x * title:          value                                 --textfield
x * link:           value                                 --textfield
x * description:    value                                 --textfield -> 'Displays solr search results of: foo, bar'
x   language        value                                 --textfield --link to format: http://www.w3.org/TR/REC-html40/struct/dirlang.html#langcodes
x   copyright       value                                 --textfield
x   managingEditor  value                                 --textfield (email)
x   webMaster       value                                 --textfield (email)
o   pubDate         value
o   lastBuildDate   value
x   category        value - attributes (1) - multiple     --textfield
o   generator       value 
F   docs            value http://cyber.law.harvard.edu/rss/rss.html
o   cloud                 - attributes (5)
o   ttl             value
x   image                 - sub-elements (3)              --default site logo or custom upload
o   textInput             - sub-elements (4) = search bar. not really possible to combine with islandora solr search.
o   skipHours             - sub-elements (x)
o   skipDays              - sub-elements (x)




Item
====
x * title        value                            --field dropdown (default: dc.title)
F * link         value                            --base url + object url + PID
x * description  value                            --field dropdown (default: dc.description)
x   author       value                            --field dropdown (default: dc.author)
F   guid         value - attributes (1)              --value: fixed (PID) attributes: fixed ('isPermaLink' => 'false')
x   enclosure            attributes (3) - multiple   --textfield (2) dropdown for file types
x   pubDate      value                                --field dropdown (default:fgs.createdDate)
x   category     value - attributes (1) - multiple    --field dropdown --attributes: field dropdown (default:dc.subject)
x   comments     value                                --textfield
x   source       value - attributes (1)               --field dropdown (2)

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
        
        
        
        
        
        
