////////////////////////////////////////////////////////////////////////////////
//
//  IDEUM
//  Copyright 2011-2013 Ideum
//  All Rights Reserved.
//
//  Gestureworks Core
//
//  File:     Game1.cs
//  Authors:  Ideum
//
//  NOTICE: Ideum permits you to use, modify, and distribute this file only
//  in accordance with the terms of the license agreement accompanying it.
//
////////////////////////////////////////////////////////////////////////////////

using System;
using System.Collections.Generic;
using System.Linq;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Audio;
using Microsoft.Xna.Framework.Content;
using Microsoft.Xna.Framework.GamerServices;
using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework.Input;
using Microsoft.Xna.Framework.Media;
using GestureWorksCoreNET;

namespace GestureWorksTutorial3
{
    /// <summary>
    /// This is the main type for your game
    /// </summary>
    public class Game1 : Microsoft.Xna.Framework.Game
    {
        GraphicsDeviceManager graphics;
        SpriteBatch spriteBatch;

        GestureWorks _gestureWorks = new GestureWorks();
        List<TouchObject> _touchObjects = new List<TouchObject>();
        Texture2D _textureGwC;

        public Game1()
        {
            graphics = new GraphicsDeviceManager(this);
            Content.RootDirectory = "Content";

            graphics.PreferredBackBufferWidth  = 1920;
            graphics.PreferredBackBufferHeight = 1080;
            graphics.ToggleFullScreen();
        }

        /// <summary>
        /// Allows the game to perform any initialization it needs to before starting to run.
        /// This is where it can query for any required services and load any non-graphic
        /// related content.  Calling base.Initialize will enumerate through any components
        /// and initialize them as well.
        /// </summary>
        protected override void Initialize()
        {
            // TODO: Add your initialization logic here
            _gestureWorks.LoadGestureWorksDll("C:\\path\\to\\GestureworksCore*.dll");
            _gestureWorks.InitializeGestureWorks(1920, 1080);
            _gestureWorks.LoadGML("C:\\path\\to\\basic_manipulation.gml");
            _gestureWorks.RegisterWindowForTouch(this.Window.Handle);

            base.Initialize();
        }

        /// <summary>
        /// LoadContent will be called once per game and is the place to load
        /// all of your content.
        /// </summary>
        protected override void LoadContent()
        {
            // Create a new SpriteBatch, which can be used to draw textures.
            spriteBatch = new SpriteBatch(GraphicsDevice);

            // TODO: use this.Content to load your game content here
            Texture2D textureXna = Content.Load<Texture2D>("logo_gestureworks_xna");

            _touchObjects.Add(new TouchObject(
                            "logo1",
                            textureXna,
                            new Vector2(graphics.GraphicsDevice.Viewport.Width / 1.50f,
                                        graphics.GraphicsDevice.Viewport.Height / 2)));

            _touchObjects.Add(new TouchObject(
                            "logo2",
                            textureXna,
                            new Vector2(graphics.GraphicsDevice.Viewport.Width / 3.00f,
                                        graphics.GraphicsDevice.Viewport.Height / 2)));

            foreach (TouchObject to in _touchObjects)
            {
                _gestureWorks.RegisterTouchObject(to.Name);
                _gestureWorks.AddGesture(to.Name, "n-drag");
                _gestureWorks.AddGesture(to.Name, "n-scale");
                _gestureWorks.AddGesture(to.Name, "n-rotate");
            }

            _textureGwC = Content.Load<Texture2D>("logo_gestureworks_core");
        }

        /// <summary>
        /// UnloadContent will be called once per game and is the place to unload
        /// all content.
        /// </summary>
        protected override void UnloadContent()
        {
            // TODO: Unload any non ContentManager content here
        }

        /// <summary>
        /// Allows the game to run logic such as updating the world,
        /// checking for collisions, gathering input, and playing audio.
        /// </summary>
        /// <param name="gameTime">Provides a snapshot of timing values.</param>
        protected override void Update(GameTime gameTime)
        {
            // Allows the game to exit
            if (Keyboard.GetState().IsKeyDown(Keys.Escape))
                this.Exit();

            // TODO: Add your update logic here
            _gestureWorks.ProcessFrame();

            GestureEventArray gestureEvents = _gestureWorks.ConsumeGestureEvents();
            PointEventArray   pointEvents   = _gestureWorks.ConsumePointEvents();

            foreach (PointEvent pointEvent in pointEvents)
            {
                if (pointEvent.Status == TouchStatus.TOUCHADDED)
                {
                    foreach (TouchObject to in _touchObjects)
                    {
                        if (Collision(pointEvent.Position, to))
                        {
                            _gestureWorks.AddTouchPoint(to.Name, pointEvent.PointId);
                        }
                    }
                }
            }

            foreach (GestureEvent gesture in gestureEvents)
            {
                foreach (TouchObject to in _touchObjects)
                {
                    if (to.Name == gesture.Target)
                    {
                        if (gesture.GestureID == "n-drag")
                        {
                            to.Position = new Vector2(to.Position.X + gesture.Values["drag_dx"],
                                                      to.Position.Y + gesture.Values["drag_dy"]);
                        }
                        else if (gesture.GestureID == "n-scale")
                        {
                            to.Scale += gesture.Values["scale_dsx"];
                        }
                        else if (gesture.GestureID == "n-rotate")
                        {
                            HandleRotate(to, gesture);
                        }
                    }
                }
            }

            base.Update(gameTime);
        }

        /// <summary>
        /// This is called when the game should draw itself.
        /// </summary>
        /// <param name="gameTime">Provides a snapshot of timing values.</param>
        protected override void Draw(GameTime gameTime)
        {
            GraphicsDevice.Clear(Color.DarkGray);

            // TODO: Add your drawing code here
            spriteBatch.Begin(SpriteSortMode.BackToFront, BlendState.AlphaBlend);

            spriteBatch.Draw(_textureGwC, Vector2.Zero, Color.White);

            foreach (TouchObject to in _touchObjects)
            {
                to.Draw(spriteBatch);
            }

            spriteBatch.End();

            base.Draw(gameTime);
        }

        /// <summary>
        /// Returns a value indicating whether the specified TouchPoint lies within the specified TouchObject's bounding box.
        /// </summary>
        /// <param name="touchPoint">TouchPoint to test for TouchObject collision.</param>
        /// <param name="touchObject">TouchObject to test for TouchPoint collision.</param>
        /// <returns><c>true</c> if the TouchPoint is within the TouchObject's bounding box, otherwise <c>false</c></returns>
        private bool Collision(TouchPoint touchPoint, TouchObject touchObject)
        {
            //Setup a bounding box for the touch point
            Rectangle touchPointBox = new Rectangle((int)touchPoint.X - 10,
                                                    (int)touchPoint.Y - 10,
                                                    20,
                                                    20);

            //Test to see if the touch point's bounding box intersects with the TouchObject
            if (touchPointBox.Intersects(touchObject.BoundingBox))
                return true;
            else
                return false;
        }

        /// <summary>
        /// Updates the position of the specified <see cref="TouchObject"/> to match that represented by the <see cref="GestureEvent"/> coordinates.
        /// </summary>
        /// <param name="touchObject">The <see cref="TouchObject"/> to rotate.</param>
        /// <param name="gesture">The <see cref="GestureEvent"/> containing the new position data.</param>
        private void HandleRotate(TouchObject touchObject, GestureEvent gesture)
        {
            //Since GestureWorks uses degrees, we have to convert to radians for XNA usage
            float theta = MathHelper.ToRadians(gesture.Values["rotate_dtheta"]);

            //Specify the rotation value of the GO
            touchObject.Rotation += theta;

            //Only update the position if there are two or more TouchPoints associated with the gesture
            if (gesture.N > 1)
            {
                //Rotate the TouchObject around the center of the Gesture, not the object
                touchObject.Position = RotateAboutCenter(touchObject.Position.X, touchObject.Position.Y, gesture.X, gesture.Y, theta);
            }
        }

        /// <summary>
        /// Returns a Vector2 for use as a position value when rotating. This takes into account the current position of the
        /// TouchObject and the position of the gesture and returns a vector appropriate for use as a new position for the TouchObject.
        /// Essentially, we want to rotate around the center of the gesture, not the object.
        /// </summary>
        /// <param name="currentX">Current X coordinate of <see cref="TouchObject"/> position.</param>
        /// <param name="currentY">Current Y coordinate of <see cref="TouchObject"/> position.</param>
        /// <param name="gestureX">The X coordinate of the <see cref="Gesture"/>.</param>
        /// <param name="gestureY">The Y coordinate of the <see cref="Gesture"/>.</param>
        /// <param name="angle">The rotation delta (in radians) obtained from GestureWorks.</param>
        /// <returns>Vector2 suitable for position coordinates.</returns>
        private Vector2 RotateAboutCenter(float currentX, float currentY, float gestureX, float gestureY, float angle)
        {
            Vector2 position = Vector2.Zero;

            double local_x = (double)(currentX - gestureX);
            double local_y = (double)(currentY - gestureY);

            double length = Math.Sqrt(local_x * local_x + local_y * local_y);

            position.X = (int)(length * Math.Cos((double)angle + Math.Atan2(local_y, local_x)) + (double)gestureX);
            position.Y = (int)(length * Math.Sin((double)angle + Math.Atan2(local_y, local_x)) + (double)gestureY);

            return position;
        }
    }
}
