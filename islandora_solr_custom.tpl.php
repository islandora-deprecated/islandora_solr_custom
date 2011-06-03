<?php
// $Id $
/**
 * @file islandora_solr_custom.tpl.php
 * Islandora solr search results template
 *
 * Variables available:
 * - $base_url: The base url of the current website. eg: http://example.com .
 * - $user: The user object.
 *
 */
?>
<ul class="islandora_solr_results">
<?php foreach ($results as $id => $result): ?>
  <li class="islandora_solr_result">
  
    <div class="solr-field <?php print $result['dc.title']['class']; ?>">  
      <label><?php print t($result['dc.title']['label']); ?></label>
      <div class="value"><?php print $result['dc.title']['value']; ?></div>
    </div>

  </li>
<?php endforeach; ?>
</ul>

<?php dsm($variables);
