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

namespace GestureWorksTutorial2
{
    /// <summary>
    /// This is the main type for your game
    /// </summary>
    public class Game1 : Microsoft.Xna.Framework.Game
    {
        GraphicsDeviceManager graphics;
        SpriteBatch spriteBatch;
        Texture2D _texture;
        SpriteFont _arialFont;

        GestureWorks _gestureWorks = new GestureWorks();
        List<PointEvent> _activePoints = new List<PointEvent>();

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
            _texture = Content.Load<Texture2D>("ring_xna");
            _arialFont = Content.Load<SpriteFont>("Arial");
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

            foreach (PointEvent point in _gestureWorks.ConsumePointEvents())
            {
                PointEvent existingPoint = _activePoints.Find(pt => pt.PointId == point.PointId);

                switch (point.Status)
                {
                    case TouchStatus.TOUCHUPDATE:
                        if (existingPoint != null)
                            existingPoint.Position = point.Position;
                        break;
                    case TouchStatus.TOUCHADDED:
                        _activePoints.Add(point);
                        break;
                    case TouchStatus.TOUCHREMOVED:
                        if (existingPoint != null)
                            _activePoints.Remove(existingPoint);
                        break;
                    default:
                        break;
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

            foreach (PointEvent point in _activePoints)
            {
                //Draw the circles at the touch points
                spriteBatch.Draw(
                    _texture,
                    new Vector2(point.Position.X, point.Position.Y),
                    null, //source rectangle
                    Color.White,
                    0, //rotation
                    new Vector2(_texture.Width / 2, _texture.Height / 2), //origin
                    1.0f, //scale
                    SpriteEffects.None,
                    0f); //layer depth

                //Draw the position text above and to the right of the touch points
                string positionString = String.Format("ID: {0}\r\nX: {1} Y: {2}", point.PointId.ToString(), ((int)point.Position.X).ToString(), ((int)point.Position.Y).ToString());

                Vector2 positionStringCenter = _arialFont.MeasureString(positionString) / 2;

                spriteBatch.DrawString(
                    _arialFont,
                    positionString,
                    new Vector2(point.Position.X + 80, point.Position.Y - 40),
                    Color.FromNonPremultiplied(96, 153, 6, 255),
                    0,
                    positionStringCenter,
                    0.75f,
                    SpriteEffects.None,
                    0.5f);
            }

            spriteBatch.End();

            base.Draw(gameTime);
        }
    }
}
