<?php

/**
 * @file islandora-solr-custom-rss.tpl.php
 * Islandora solr search rss results template
 *
 * Variables available:
 * - $variables: all array elements of $variables can be used as a variable. e.g. $base_url equals $variables['base_url']
 * - $base_url: The base url of the current website. eg: http://example.com .
 * - $user: The user object.
 *
 * - $style: the style of the display ('div' or 'table'). Set in admin page by default. Overridden by the query value: ?display=foo
 * - $results: the array containing the solr search results
 * - $table_rendered: If the display style is set to 'table', this will contain the rendered table.
 *    For theme overriding, see: theme_islandora_solr_custom_table() 
 * - $switch_rendered: The rendered switch to toggle between display styles
 *    For theme overriding, see: theme_islandora_solr_custom_switch() 
 *
 */
?>

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

