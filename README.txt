////////////////////////////////Important!/////////////////////////////////////
This module is an addon module for the islandora_solr_search module. There are
various versions of islandora_solr_search module around, but it's important to
use the latest stable version.
You can find it here: https://github.com/Islandora/islandora_solr_search
///////////////////////////////////////////////////////////////////////////////


The islandora_solr_custom module provides a display type for the islandora_solr_search module.
It automatically looks for the fields set for display in solrconfig.xml and
outputs the fields in the order that they are defined in the requestHandler.
Islandora_solr_custom will look at the settings of islandora_solr_search to find
solrconfig.xml and the requestHandler. You can quickly customize the output 
style of your search results by changing the settings in the admin section 
or you can take full control by overriding the template file.


TODO
====

- get the reset functionality in the admin page fixed up.

- RSS functionality works well, but could be better/fancier?. Right now if you 
want to create settings for multiple different RSS feeds, you'll have to 
overwrite a theme function and set new values to the variables using a 
conditional statement of some sorts. There was the option to have it 'pluggable'
trough a function in a module using module_invoke_all(), but you would have to
write an entire module to set some values. Another option would be to make a 
user interface for this, but to make this any powerful, it would require a 
pretty complex module. We're dealing with multiple RSS elements like <enclosure>
and <category>. Some items you might want to see conditional, for example add
a different <enclosure> tag dependent on the Object type. This would require
some php tweaking which is only possible in a theme file or module.

Another option I might look into is in the admin interface, provide a textarea
with some of the basic php code as default. Users can modify those settings, which 
will be displayed in a list. It not really great working with php snippets in
the admin interface, but popular modules like Display Suite and Custom Formatters
provide this interface and I've worked lots with it. It's not too bad.

- look into merging options with islandora_solr_search:

  - I think the default display should provide a template file.
  
  - A table display could be default as well.
  
  - The primary display selection list could be a table with the following columns:
    - checkboxes to enable multiple displays (enabled displays would get a switch
      link. I don't see a scenario where you want to enable a display without
      the ability to switch to it)
    - radio buttons to specify one default display
    
  - I don't know about the functionality where you can overwrite labels etc.
    It might be a bit too complex and I don't know how flexible it is. It works
    for me, but I don't know how useful it is to others. 
    But if it's included: the search terms and facet fields selection could
    easily added as 2 extra columns with checkboxes.
    
  - If there is a merge, I'm not sure where the template code generator would go.
    It's specific to one display style, so it won't be appropriate on the main
    page. Probably a separate page would be best. The css inclusion could go
    on the same page. The RSS settings would need a separate page as well.
    Probably best to create child pages instead of sibling pages for that.
    
  - I'm wondering if the 'results per page' could be set per (primary) display. A
    table could take more results than a div based layout for example. This
    could also be added to the primary display table, maybe? Or would that get
    too messy? It could also go on a settings page for that display where it
    can overwrite the default.
    


aggregator patch:
=================

To insert rss feeds into views you can use the core aggregator or the feeds module.
If you use the core aggregator you can render the rss elements into views fields.
To get the GUID element in a field which contains the PID, views 6.x-2.12 needs
a patch: http://drupal.org/node/848506 This will be added into 6.x-2.13, but
that will take a while before it's out.
Once you get the PID, you can theme almost every media element of the object.

In addition to that the Item ID patch may come in handy too.
http://drupal.org/node/848506

Reminders:
==========

* = required
x = useful
o = not going to use
F = Fixed value
B = Basic settings
A = Advanced settings

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
B x * title:          value                                 --textfield
B x * link:           value                                 --textfield
B x * description:    value                                 --textfield -> 'Displays solr search results of: foo, bar'
A x   language        value                                 --textfield --link to format: http://www.w3.org/TR/REC-html40/struct/dirlang.html#langcodes
B F   docs            value http://cyber.law.harvard.edu/rss/rss.html
B x   image                 - sub-elements (3)              --default site logo or custom upload
A x   copyright       value                                 --textfield
A x   managingEditor  value                                 --textfield (email)
A x   webMaster       value                                 --textfield (email)
A x   category        value - attributes (1) - multiple     --textfield
o   pubDate         value
o   lastBuildDate   value
o   generator       value 
o   cloud                 - attributes (5)
o   ttl             value
o   textInput             - sub-elements (4) = search bar. not really possible to combine with islandora solr search.
o   skipHours             - sub-elements (x)
o   skipDays              - sub-elements (x)




Item
====
B x * title        value                            --field dropdown (default: dc.title)
B F * link         value                            --base url + object url + PID
B x * description  value                            --field dropdown (default: dc.description)
B F   guid         value - attributes (1)              --value: fixed (PID) attributes: fixed ('isPermaLink' => 'false')
B x   enclosure            attributes (3) - multiple   --textfield (2) dropdown for file types
A x   author       value                            --field dropdown (default: dc.author)
A x   pubDate      value                                --field dropdown (default:fgs.createdDate)
A x   category     value - attributes (1) - multiple    --field dropdown --attributes: field dropdown (default:dc.subject)
A x   comments     value                                --textfield
A x   source       value - attributes (1)               --field dropdown (2)

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
        
        
        
        
This is how an rss template would look like. (roughly)
        
<?php print "<?xml"; ?> version="1.0" encoding="utf-8" <?php print "?>"; ?>
<rss version="2.0" xml:base="<?php print $link; ?>"<?php print $namespaces; ?>>
  <channel>
    <title><?php print $viewtitle; ?></title>
    <description><?php print $description; ?></description>
    <link><?php print $link ?></link>
    
    <language></language>
    <copyright></copyright>
    <managingEditor></managingEditor>
    <webMaster></webMaster>
    <pubDate></pubDate>
    <lastBuildDate></lastBuildDate>
    <category></category>
    <generator></generator>
    <docs></docs>
    <cloud />
    <ttl></ttl>
    <image>
      <url></url>
      <title></title>
      <link></link>
      <width></width>
      <height></height>
      <description></description>
    </image>
    <textInput>
      <title></title>
      <description></description>
      <name></name>
      <link></link>
    </textInput>
    <skipHours>
      <hour></hour>
    </skipHours>
    <skipDays>
      <day></day>
    </skipDays>
    
    <?php foreach ($whatever as $key => $value): ?>
      <item>
        <title><?php print $title; ?></title>
        <link><?php print $link; ?></link>
        <description><?php print $description; ?></description>
        
        <author></author>
        <guid></guid>
        <enclosure />
        <pubDate></pubDate>
        <category></category>
        <comments></comments>
        <source></source>
      </item>
    <?php endforeach; ?>
  
  </channel>
</rss>
        
