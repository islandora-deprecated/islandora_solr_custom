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
  <?php foreach ($result as $field => $values): ?> 
    <?php 
      $value = $values['value'];
      $label = $values['label'];
      $class = $values['class'];
      $exclude_label = $values['exclude_label'];
      $markup = $values['markup'];
      $zebra = $values['zebra'];
    ?>
    <?php if($markup == 1): ?>
      <div class="solr-field <?php print $class.' '.$zebra ?>">  
        <?php if($exclude_label == 0): ?>
          <label><?php print t($label); ?></label>
        <?php endif; ?>
        <div class="value"><?php print $value; ?></div>
      </div>
    <?php endif; ?>
    
  <?php endforeach; ?>
  </li>
<?php endforeach; ?>
</ul>

<?php dsm($variables);
