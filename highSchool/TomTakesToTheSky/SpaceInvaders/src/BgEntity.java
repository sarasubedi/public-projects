public class BgEntity extends Entity {

  private Game game; // the game in which the background exists

  /* construct a new background
   * input: game - the game in which the background is being created
   *        r - the image representing the background
   *        x, y - initial location of background
   */
  public BgEntity(Game g, String r, int newX, int newY) {
    super(r, newX, newY);  // calls the constructor in Entity
    game = g;
    
  } // constructor
  
  /* collidedWith
   * input: other - the entity with which the background has collided
   * purpose: required in the child classes of Entity
   */
   public void collidedWith(Entity other) {
     // collisions aren't needed for background
   } // collidedWith
  
} // BgEntity